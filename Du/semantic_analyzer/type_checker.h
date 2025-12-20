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
#include "../ast/AstControlBlock.h"
#include "../ast/CastGraph.hpp"
#include "../ast/AstBuildSystem.hpp"
#include "../ast/VariableDecorator.hpp" 
#include "../ast/AstReference.h"      
#include "../ast/AstVariableDecl.hpp" 
#include "../Terminal/Terminal.hpp"
#include "../ast/AstVisitor.hpp"

// --- TYPE RESOLVER VISITOR ---
struct TypeResolverVisitor {
    AstScope* currentScope;

    TypeResolverVisitor(AstScope* scope) : currentScope(scope) {}

    std::optional<BasicTypes> visit(AstExpr* expr) {
        if (!expr) return std::nullopt;
        if (auto t = expr->getType()) return t->getType(); // If already set
        return expr->accept(*this);
    }

    std::optional<BasicTypes> operator()(AstNodes::LiteralExpr& lit) {
        return std::visit(overloaded {
            [](long long) -> std::optional<BasicTypes> { return BasicTypes::U64; },
            [](unsigned long long) -> std::optional<BasicTypes> { return BasicTypes::U64; },
            [](double) -> std::optional<BasicTypes> { return BasicTypes::F32; },
            [](bool) -> std::optional<BasicTypes> { return BasicTypes::BOOL; },
            [](const std::string&) -> std::optional<BasicTypes> { return BasicTypes::STR; }
        }, lit.value);
    }

    std::optional<BasicTypes> operator()(AstNodes::VariableRefExpr& ref) {
        if (auto decl = ast_element_cast<AstVariableDecl>(ref.declaration)) {
            if (auto type = decl->getVarType()) return type->getType();
        }
        return std::nullopt; 
    }

    std::optional<BasicTypes> operator()(AstNodes::ArrayIndexingExpr& arrIndex) {
        AstVariableDecl* varDecl = nullptr;
        
        if (auto* baseExpr = arrIndex.arrayExpr.get()) {
             // Resolve base (assuming variable ref for now)
             // We need to peek into baseExpr manually or use a helper
             // But baseExpr is AstExpr. We can use this visitor recursively?
             // No, baseExpr stores VariableRefExpr.
             // Let's access it directly via visit on baseExpr
             // But we need the DECLARATION, not just type.
             // visitor returns type.
             
             // So we need manual access here:
             auto& exprVariant = baseExpr->getExpression();
             if (auto* v = std::get_if<AstNodes::VariableRefExpr>(&exprVariant)) {
                 varDecl = ast_element_cast<AstVariableDecl>(v->declaration);
             }
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
    }

    std::optional<BasicTypes> operator()(AstNodes::FunctionCallExpr& call) {
        // TODO: Lookup function return type
        return std::nullopt; 
    }

    std::optional<BasicTypes> operator()(AstNodes::BinaryExpr& binOp) {
        // Simple logic: result type is usually same as operands (after check)
        auto l = visit(binOp.left.get());
        auto r = visit(binOp.right.get());
        if (l && r && *l == *r) return *l;
        // If diff, assume checkExpr handled promotion and set Parent Type
        return std::nullopt; 
    }

    std::optional<BasicTypes> operator()(AstNodes::CmpExpr&) {
        return BasicTypes::BOOL;
    }

    std::optional<BasicTypes> operator()(AstNodes::CastExpr& cast) {
        // CastExpr should have explicit type set usually? 
        // No, CastExpr casts TO some type. That type should be stored in AstExpr parent meta.
        // But wait, AstExpr::getType() is checked at start of visit().
        return std::nullopt;
    }

    template <typename T>
    std::optional<BasicTypes> operator()(T&) { return std::nullopt; }
};


// --- TYPE CHECKER VISITOR ---
struct TypeCheckVisitor {
    AstScope* currentScope;
    AstExpr* parentExpr; // The expr holding the variant being visited

    TypeCheckVisitor(AstScope* scope, AstExpr* parent) 
        : currentScope(scope), parentExpr(parent) {}

    // Helpers
    int getTypeRank(BasicTypes bt) {
        switch(bt) {
            case BasicTypes::BOOL: return 1;
            case BasicTypes::I8:
            case BasicTypes::U8: return 2;
            case BasicTypes::I16:
            case BasicTypes::U16: return 3;
            case BasicTypes::I32:
            case BasicTypes::U32: return 4;
            case BasicTypes::F32: return 5;
            case BasicTypes::I64:
            case BasicTypes::U64: return 6;
            case BasicTypes::F64: return 7;
            default: return 0;
        }
    }

    bool tryPromote(std::unique_ptr<AstExpr>& childExpr, BasicTypes targetType, BasicTypes sourceType) {
		if (targetType == sourceType) return true;

		CastOp op = CastGraph::getCastOp(sourceType, targetType);
		if (op != CastOp::NoOp) {
			auto& factory = AstBuildSystem::Instance().getFactory();
			AstExpr* rawChild = childExpr.release();
			auto castExprUnique = factory.exprFactor().createCast(rawChild, op);
            castExprUnique->setType(std::make_unique<AstType>(targetType)); 
            childExpr = std::move(castExprUnique);
			return true;
		}
		return false;
    }

    // Recursive entry point
    void check(AstExpr* expr) {
        if (!expr) return;
        TypeCheckVisitor childVisitor(currentScope, expr);
        expr->accept(childVisitor);
    }

    std::optional<BasicTypes> resolve(AstExpr* expr) {
        TypeResolverVisitor resolver(currentScope);
        return resolver.visit(expr);
    }

    // --- VISIT METHODS ---

    void operator()(AstNodes::BinaryExpr& binOp) {
        std::cout << "Checker: Binary L: " << binOp.left.get() << " R: " << binOp.right.get() << std::endl;
        check(binOp.left.get());
        check(binOp.right.get());

        auto lTypeOpt = resolve(binOp.left.get());
        auto rTypeOpt = resolve(binOp.right.get());

        if (!lTypeOpt || !rTypeOpt) return; // Error logged elsewhere?

        BasicTypes lType = *lTypeOpt;
        BasicTypes rType = *rTypeOpt;

        if (lType == rType) {
            parentExpr->setType(std::make_unique<AstType>(lType));
            return;
        }

        int lRank = getTypeRank(lType);
        int rRank = getTypeRank(rType);

        if (lRank < rRank) { 
            if (tryPromote(binOp.left, rType, lType)) {
                parentExpr->setType(std::make_unique<AstType>(rType));
                return;
            }
        } else if (rRank < lRank) { 
            if (tryPromote(binOp.right, lType, rType)) {
                parentExpr->setType(std::make_unique<AstType>(lType));
                return;
            }
        }
        std::cerr << "Type Error: Incompatible types\n";
    }

    void operator()(AstNodes::CmpExpr& cmpOp) {
        check(cmpOp.left.get());
        check(cmpOp.right.get());
        
        auto lTypeOpt = resolve(cmpOp.left.get());
        auto rTypeOpt = resolve(cmpOp.right.get());
        
        if (!lTypeOpt || !rTypeOpt) return;

        BasicTypes lType = *lTypeOpt;
        BasicTypes rType = *rTypeOpt;

        if (lType == rType) {
            parentExpr->setType(std::make_unique<AstType>(BasicTypes::BOOL));
            return;
        }

        int lRank = getTypeRank(lType);
        int rRank = getTypeRank(rType);

        if (lRank < rRank) tryPromote(cmpOp.left, rType, lType);
        else if (rRank < lRank) tryPromote(cmpOp.right, lType, rType);
        
        parentExpr->setType(std::make_unique<AstType>(BasicTypes::BOOL));
    }

    void operator()(AstNodes::UnaryExpr& unOp) {
        check(unOp.operand.get());
        auto type = resolve(unOp.operand.get());
        if(type) parentExpr->setType(std::make_unique<AstType>(*type));
    }

    void operator()(AstNodes::VariableRefExpr& ref) {
        auto type = resolve(parentExpr); // resolve uses parentExpr directly via declaration
        if(type) parentExpr->setType(std::make_unique<AstType>(*type));
    }

    void operator()(AstNodes::LiteralExpr& lit) {
        // Default resolution for literals (e.g. U64)
        auto type = resolve(parentExpr);
        if(type) parentExpr->setType(std::make_unique<AstType>(*type));
    }

    void operator()(AstNodes::CastExpr& cast) {
        check(cast.expr.get());
        // CastExpr type should be set by whoever created it
    }

    void operator()(AstNodes::FunctionCallExpr& call) {
        for(auto& arg : call.args) check(arg.get());
    }

    void operator()(AstNodes::ArrayIndexingExpr& idx) {
        // check base?
        if(idx.arrayExpr) check(idx.arrayExpr.get());
        for(auto& i : idx.indices) check(i.get());

        auto type = resolve(parentExpr);
        if(type) parentExpr->setType(std::make_unique<AstType>(*type));
    }
};


class TypeChecker
{
private:
	AstScope* m_mainScope;
	AstScope* m_currentScope;

    int getTypeRank(BasicTypes bt) {
        // Duplicate helper, or move to static/utils
        switch(bt) {
            case BasicTypes::BOOL: return 1;
            case BasicTypes::I8:
            case BasicTypes::U8: return 2;
            case BasicTypes::I16:
            case BasicTypes::U16: return 3;
            case BasicTypes::I32:
            case BasicTypes::U32: return 4;
            case BasicTypes::F32: return 5;
            case BasicTypes::I64:
            case BasicTypes::U64: return 6;
            case BasicTypes::F64: return 7;
            default: return 0;
        }
    }

	std::optional<BasicTypes> resolveType(AstElement* node)
	{
		if (!node) return std::nullopt;

		if (auto decl = ast_element_cast<AstVariableDecl>(node))
		{
			if (auto type = decl->getVarType()) return type->getType();
		}
		else if (auto funScope = ast_element_cast<AstScope>(node))
		{
            std::optional<BasicTypes> ret;
            funScope->accept(overloaded{
                [&](ScopeDecorator::Function& func) -> void
                {
                    ret = func.getRetType();
                },
                [&](auto&&) -> void
                {
                    ret = std::nullopt;
                }
				});
            return ret;

		}
		else if (auto expr = ast_element_cast<AstExpr>(node))
		{
            TypeResolverVisitor resolver(m_currentScope);
            return resolver.visit(expr);
		}
		return std::nullopt;
	}

	void analyzeScope(AstScope* scope)
	{
		if (!scope) return;
		AstScope* prevScope = m_currentScope;
		m_currentScope = scope;

        auto stmts = scope->getStmts();
        for (auto&& it : stmts)
        {
            if (auto stmt = ast_unique_element_cast<AstStatement>(it))
            {
                // 1. Check RHS expression (bottom-up type inference)
                TypeCheckVisitor checker(m_currentScope, stmt->rhs());
                if(stmt->rhs())
                    stmt->rhs()->accept(checker);

                // 2. Check assignments / declarations
                std::optional<BasicTypes> optType = std::nullopt;

				if (stmt->getStmtType() == AstStatement::STMT_TYPE::CONDITION_BLOCK || stmt->getStmtType() == AstStatement::STMT_TYPE::LOOP_BLOCK)
                {
                    if (stmt->isControlBlockStmt())
                    {
                        auto& controlBlock = stmt->getControlBlock();
                        if (controlBlock)
                        {
                            auto conditionExpr = stmt->rhs();
                            TypeCheckVisitor condChecker(m_currentScope, conditionExpr);
                            conditionExpr->accept(condChecker);

                            if (conditionExpr && conditionExpr->getType())
                            {
                                BasicTypes condType = conditionExpr->getType()->getType();
                                if (condType != BasicTypes::BOOL)
                                {
                                    CastOp op = CastGraph::getCastOp(condType, BasicTypes::BOOL);
                                    if (op != CastOp::NoOp)
                                    {
                                        auto cast = AstBuildSystem::Instance().getFactory().exprFactor().createCast(stmt->releaseRhs().release(), op);
                                        cast->setType(std::make_unique<AstType>(BasicTypes::BOOL));
                                        stmt->setWrappedRhs(std::move(cast));
                                    }
                                    else
                                    {
                                        Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::CodeList::DU003, "Condition must be boolean convertible");
                                    }
                                }
                            }

                            analyzeScope(controlBlock->getBranchScope().get());
                            analyzeScope(controlBlock->getOtherBranch().get());
                        }
                    }
                }
                else if (stmt->getStmtType() == ::AstStatement::STMT_TYPE::RET_STMT)
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
                        // Special handling for Literals: change type
                        bool isLiteral = std::holds_alternative<AstNodes::LiteralExpr>(stmt->rhs()->getExpression());

                        if (isLiteral) {
                            stmt->rhs()->setType(std::make_unique<AstType>(targetType));
                        } else {
                            // Insert Cast
                            CastOp op = CastGraph::getCastOp(rhsType, targetType);
                            
                            if (op == CastOp::NoOp) {
                                    int rRank = getTypeRank(rhsType);
                                    int tRank = getTypeRank(targetType);
                                    if (rRank > 0 && tRank > 0) {
                                        if (rRank > tRank) op = CastOp::Trunc;
                                        else if (rRank < tRank) op = CastOp::SExt; 
                                    }
                            }

                            if (op != CastOp::NoOp) {
                                auto cast = AstBuildSystem::Instance().getFactory().exprFactor().createCast(stmt->releaseRhs().release(), op);
                                cast->setType(std::make_unique<AstType>(targetType));
                                stmt->setWrappedRhs(std::move(cast));
                            }
                        }
                    }
                }
            }
        }

		auto elements = scope->getElements();
		for (auto&& it : elements)
		{
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
