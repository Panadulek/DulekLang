#pragma once

#include <optional>
#include <iostream> 
#include <vector>
#include <variant>
#include <memory>

#include "../ast/AstScope.hpp"
#include "../ast/AstExpr.hpp"
#include "../ast/AstElement.hpp"
#include "../ast/AstType.hpp"
#include "../ast/CastGraph.hpp"
#include "../ast/AstBuildSystem.hpp"
#include "../ast/VariableDecorator.hpp" 
#include "../ast/AstReference.h"      
#include "../ast/AstVariableDecl.hpp" 
#include "../Terminal/Terminal.hpp"
#include "../ast/AstVisitor.hpp"

class TypeChecker
{
private:
	AstScope* m_mainScope;
	AstScope* m_currentScope;

	std::optional<BasicTypes> resolveType(AstElement* node)
	{
		if (!node) return std::nullopt;

		if (auto decl = ast_element_cast<AstVariableDecl>(node))
		{
			if (auto type = decl->getVarType()) return type->getType();
		}
		else if (auto funScope = ast_element_cast<AstScope>(node))
		{
			if (auto funData = funScope->getFunctionDecorator()) return funData->getRetType();
		}
		else if (auto expr = ast_element_cast<AstExpr>(node))
		{
			if (auto t = expr->getType()) return t->getType();

            return std::visit(overloaded {
                [](const AstNodes::LiteralExpr& lit) -> std::optional<BasicTypes> { 
                    return std::visit(overloaded{
                        [](long long) -> std::optional<BasicTypes> { return BasicTypes::U64; },
                        [](unsigned long long) -> std::optional<BasicTypes> { return BasicTypes::U64; },
                        [](double) -> std::optional<BasicTypes> { return BasicTypes::F32; },
                        [](bool) -> std::optional<BasicTypes> { return BasicTypes::BOOL; },
                        [](const std::string&) -> std::optional<BasicTypes> { return BasicTypes::STR; }
                    }, lit.value);
                },
                [](const AstNodes::VariableRefExpr& ref) -> std::optional<BasicTypes> { 
                     if (auto decl = ast_element_cast<AstVariableDecl>(ref.declaration)) {
                         if (auto type = decl->getVarType()) return type->getType();
                     }
                     return std::nullopt; 
                },
                [&](const AstNodes::ArrayIndexingExpr& arrIndex) -> std::optional<BasicTypes> {
                    AstVariableDecl* varDecl = nullptr;
                    
                    if (auto* baseExpr = arrIndex.arrayExpr.get()) {
                         std::visit(overloaded {
                            [&](const AstNodes::VariableRefExpr& v) {
                                varDecl = ast_element_cast<AstVariableDecl>(v.declaration);
                            },
                            [](auto&&) {}
                        }, baseExpr->getExpression());
                    }

                    if (!varDecl) return std::nullopt;

                    AstType* varType = varDecl->getVarType();
                    if (!varType || !varType->isArray()) return std::nullopt;
                    
                    size_t declared_dims = varType->getDimensionCounter();
                    size_t access_dims = arrIndex.indices.size();

                    if (declared_dims > 0 && declared_dims == access_dims) {
                        return varType->getType();
                    }
                    Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::CodeList::DU003, varDecl->getName());
                    return std::nullopt;
                },
                [](auto&&) -> std::optional<BasicTypes> { return std::nullopt; }
            }, expr->getExpression());
		}
		return std::nullopt;
	}

	bool tryPromote(std::unique_ptr<AstExpr>& childExpr, BasicTypes targetType, BasicTypes sourceType)
	{
		if (targetType == sourceType) return true;

		CastOp op = CastGraph::getCastOp(sourceType, targetType);
		if (op != CastOp::NoOp)
		{
			auto& factory = AstBuildSystem::Instance().getFactory();
			AstExpr* rawChild = childExpr.release();
			AstExpr* castExprRaw = factory.exprFactor().createCast(rawChild, op);
            childExpr.reset(castExprRaw);
            childExpr->setType(std::make_unique<AstType>(targetType)); 
			return true;
		}
		return false;
	}

	void checkBinaryOp(AstNodes::BinaryExpr& binOp, AstExpr* parentExpr)
	{
        // Bottom-Up traversal
		checkExpr(binOp.left.get());
		checkExpr(binOp.right.get());

		auto lTypeOpt = resolveType(binOp.left.get());
		auto rTypeOpt = resolveType(binOp.right.get());

		if (!lTypeOpt || !rTypeOpt) {
			std::cerr << "Type Error: Cannot resolve types for binary operation.\n";
			return;
		}

		BasicTypes lType = *lTypeOpt;
		BasicTypes rType = *rTypeOpt;

		if (lType == rType)
		{
			parentExpr->setType(std::make_unique<AstType>(lType));
			return;
		}

		if (tryPromote(binOp.right, lType, rType))
		{
			parentExpr->setType(std::make_unique<AstType>(lType));
			return;
		}

		if (tryPromote(binOp.left, rType, lType))
		{
			parentExpr->setType(std::make_unique<AstType>(rType));
			return;
		}

		std::cerr << "Type Error: Incompatible types (" << (int)lType << " vs " << (int)rType << ")\n";
	}

    void checkCmpOp(AstNodes::CmpExpr& cmpOp, AstExpr* parentExpr)
    {
        checkExpr(cmpOp.left.get());
		checkExpr(cmpOp.right.get());

		auto lTypeOpt = resolveType(cmpOp.left.get());
		auto rTypeOpt = resolveType(cmpOp.right.get());

		if (!lTypeOpt || !rTypeOpt) {
			std::cerr << "Type Error: Cannot resolve types for comparison.\n";
			return;
		}

		BasicTypes lType = *lTypeOpt;
		BasicTypes rType = *rTypeOpt;

		if (lType == rType) return;

		if (tryPromote(cmpOp.right, lType, rType)) return;
		if (tryPromote(cmpOp.left, rType, lType)) return;

		std::cerr << "Type Error: Incompatible types in cmp (" << (int)lType << " vs " << (int)rType << ")\n";
    }

	void checkExpr(AstExpr* expr)
	{
		if (!expr) return;

        std::visit(overloaded {
            [&](AstNodes::BinaryExpr& binOp) {
                checkBinaryOp(binOp, expr);
            },
            [&](AstNodes::CmpExpr& cmpOp) {
                checkCmpOp(cmpOp, expr);
            },
            [&](AstNodes::VariableRefExpr& varRef) {
                 if(auto* decl = ast_element_cast<AstVariableDecl>(varRef.declaration)) {
                     if(auto* t = decl->getVarType()) {
                         expr->setType(std::make_unique<AstType>(t->getType()));
                     }
                 }
            },
            [&](AstNodes::LiteralExpr& lit) {
                 auto typeOpt = resolveType(expr);
                 if(typeOpt) expr->setType(std::make_unique<AstType>(*typeOpt));
            },
            [&](AstNodes::CastExpr& cast) {
                checkExpr(cast.expr.get());
            },
            [&](AstNodes::FunctionCallExpr& call) {
                 // Lookup function scope/return type would go here
                 for(auto& arg : call.args) {
                     checkExpr(arg.get());
                 }
            },
            [&](AstNodes::ArrayIndexingExpr& idx) {
                auto typeOpt = resolveType(expr);
                if(typeOpt) expr->setType(std::make_unique<AstType>(*typeOpt));
            },
            [](auto&&) {}
        }, expr->getExpression());
	}

	void analyzeScope(AstScope* scope)
	{
		if (!scope) return;
		AstScope* prevScope = m_currentScope;
		m_currentScope = scope;

		auto elements = scope->getElements();
		for (auto&& it : elements)
		{
			auto stmts = scope->getStmts();
			for (auto&& it : stmts)
			{
				if (auto stmt = ast_unique_element_cast<AstStatement>(it))
				{
					checkExpr(stmt->rhs());
					std::optional<BasicTypes> optType = std::nullopt;
					if (stmt->getStmtType() == ::AstStatement::STMT_TYPE::RET_STMT)
					{
						optType = resolveType(m_currentScope);
					}
					else
					{
						optType = resolveType(stmt->lhs());
					}
                    
					if (optType.has_value() && stmt->rhs() && stmt->rhs()->getType())
					{
                        BasicTypes rhsType = stmt->rhs()->getType()->getType();
                        BasicTypes targetType = *optType;
                        
                        if (rhsType != targetType) {
						    CastOp op = CastGraph::getCastOp(rhsType, targetType);
                            if (op != CastOp::NoOp) {
    						    auto cast = AstBuildSystem::Instance().getFactory().exprFactor().createCast(stmt->rhs(), op);
						    cast->setType(std::make_unique<AstType>(targetType));
						    stmt->setWrappedRhs(cast);
                            }
                        }
					}
				}
			}
			if (auto subScope = ast_unique_element_cast<AstScope>(it))
			{
				analyzeScope(subScope); 
			}
		}

		m_currentScope = prevScope;
	}

public:
	TypeChecker(AstScope* scope)
		: m_mainScope(scope), m_currentScope(nullptr)
	{
	}

	void operator ()()
	{
		if (!m_mainScope) return;
		analyzeScope(m_mainScope);
	}
};