#pragma once

#include <optional>
#include <iostream> 
#include <vector>

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
class TypeChecker
{
private:
	AstScope* m_mainScope;

	AstScope* m_currentScope;

	std::optional<BasicTypes> resolveType(AstElement* node)
	{
		if (!node) return std::nullopt;

		if (auto ref = ast_element_cast<AstRef>(node))
		{
			return ref->getType();
		}

		else if (auto decl = ast_element_cast<AstVariableDecl>(node))
		{
			if (auto type = decl->getVarType())
			{
				return type->getType();
			}
		}

		else if (auto funScope = ast_element_cast<AstScope>(node))
		{
			if (auto funData = funScope->getFunctionDecorator())
			{
				return funData->getRetType();
			}
		}
		else if (auto expr = ast_element_cast<AstExpr>(node))
		{
			if (auto t = expr->getType()) {
				return t->getType();
			}
			else if (expr->op() == AstExpr::Operation::Arr_Indexing)
			{
				AstElement* base_ref_element = expr->left();
				AstVariableDecl* varDecl = nullptr;

				if (auto ref_expr = ast_element_cast<AstExpr>(base_ref_element)) {
					if (ref_expr->op() == AstExpr::Operation::Reference) {
						if (auto ref = ast_element_cast<AstRef>(ref_expr->right())) {
							varDecl = ast_element_cast<AstVariableDecl>(ref->ref());
						}
					}
				}
				
				if (!varDecl)
				{
					return std::nullopt;
				}

				AstType* varType = varDecl->getVarType();
				if (!varType || !varType->isArray()) return std::nullopt;
				size_t declared_dims = varType->getDimensionCounter();

				size_t access_dims = 0;
				AstExpr* index_chain = ast_element_cast<AstExpr>(expr->right());
				while (index_chain != nullptr) {
					access_dims++;
					index_chain = ast_element_cast<AstExpr>(index_chain->right());
				}
				if (declared_dims > 0 && declared_dims == access_dims) {
					return varType->getType();
				}
				Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::CodeList::DU003, varDecl->getName());
			}
		}
		return std::nullopt;
	}

	// -------------------------------------------------------------------------
	// Core Logic
	// -------------------------------------------------------------------------

	// Sprawdza i ewentualnie wstawia w�z�y rzutowania (Implicit Cast)
	bool tryPromote(AstExpr* parentExpr, bool isLeftChild, BasicTypes targetType, BasicTypes sourceType)
	{
		if (targetType == sourceType) return true;

		// Sprawd� w grafie, czy istnieje konwersja Source -> Target
		CastOp op = CastGraph::getCastOp(sourceType, targetType);

		if (op != CastOp::NoOp)
		{

			auto& factory = AstBuildSystem::Instance().getFactory();
			
			
			if (isLeftChild)
			{
				AstElement* element = parentExpr->left();
				AstExpr* castExpr = factory.exprFactor().createCast(element, op);
				castExpr->setType(new AstType(targetType));
				parentExpr->left(castExpr);
			}
			else
			{
				AstElement* element = parentExpr->right();
				AstExpr* castExpr = factory.exprFactor().createCast(element, op);
				castExpr->setType(new AstType(targetType));
				parentExpr->right(castExpr);
			}
			
			 std::cout << "Inserting Implicit Cast: " << (int)sourceType  << " -> " << (int)targetType << " (Op: " << (int)op << ")\n";

			return true;
		}

		return false;
	}

	void checkBinaryOp(AstExpr* expr)
	{
		// 1. Najpierw sprawd� dzieci (Bottom-Up traversal)
		checkExpr(reinterpret_cast<AstExpr*>(expr->left()));
		checkExpr(reinterpret_cast<AstExpr*>(expr->right()));

		// 2. Pobierz typy dzieci po sprawdzeniu
		auto lTypeOpt = resolveType(expr->left());
		auto rTypeOpt = resolveType(expr->right());

		if (!lTypeOpt || !rTypeOpt) {
			// B��d: nie uda�o si� ustali� typ�w operand�w
			std::cerr << "Type Error: Cannot resolve types for binary operation.\n";
			return;
		}

		BasicTypes lType = *lTypeOpt;
		BasicTypes rType = *rTypeOpt;

		if (lType == rType)
		{
			expr->setType(new AstType(lType));
			return;
		}

		if (tryPromote(expr, true, rType, lType))
		{
			expr->setType(new AstType(rType));
			return;
		}

		if (tryPromote(expr, false, lType, rType))
		{
			expr->setType(new AstType(lType));
			return;
		}

		std::cerr << "Type Error: Incompatible types in binary expression (" 
			<< (int)lType << " vs " << (int)rType << ")\n";
	}

	void checkCmpOp(AstExpr* expr)
	{
		// 1. Najpierw sprawd dzieci (Bottom-Up traversal)
		checkExpr(reinterpret_cast<AstExpr*>(expr->left()));
		checkExpr(reinterpret_cast<AstExpr*>(expr->right()));

		// 2. Pobierz typy dzieci po sprawdzeniu
		auto lTypeOpt = resolveType(expr->left());
		auto rTypeOpt = resolveType(expr->right());

		if (!lTypeOpt || !rTypeOpt) {
			// Bd: nie udao si ustali typw operandw
			std::cerr << "Type Error: Cannot resolve types for comparison operation.\n";
			return;
		}

		BasicTypes lType = *lTypeOpt;
		BasicTypes rType = *rTypeOpt;

		if (lType == rType)
		{
			// Typy s zgodne, nic nie trzeba robi.
			return;
		}

		// Sprbuj wypromowa lewy do typu prawego
		if (tryPromote(expr, true, rType, lType))
		{
			return;
		}

		// Sprbuj wypromowa prawy do typu lewego
		if (tryPromote(expr, false, lType, rType))
		{
			return;
		}

		// Jeli promocja niemoliwa, to bd typw.
		std::cerr << "Type Error: Incompatible types in comparison expression ("
			<< (int)lType << " vs " << (int)rType << ")\n";
	}

	void checkExpr(AstExpr* expr)
	{
		if (!expr) return;

		if (expr->isBinaryOp())
		{
			checkBinaryOp(expr);
		}
		else if (expr->isCmpOp())
		{
			checkCmpOp(expr);
		}
		else if (expr->op() == AstExpr::Operation::Reference)
		{
			if (auto ref = ast_element_cast<AstRef>(expr->right()))
			{
				auto type = ref->getType();
				if(type.has_value())
					expr->setType(new AstType(type.value()));
			}
		}
		else if (expr->op() == AstExpr::Operation::ConstValue)
		{
			if (auto constVal = ast_element_cast<AstConst>(expr->right()))
			{
				expr->setType(new AstType(constVal->getType()));
			}
		}
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
					std::optional<BasicTypes>  optType = std::nullopt;
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
						CastOp op = CastGraph::getCastOp(stmt->rhs()->getType()->getType(), *optType);
						auto cast = AstBuildSystem::Instance().getFactory().exprFactor().createCast(stmt->rhs(), op);
						cast->setType(new AstType(*optType));
						stmt->setWrappedRhs(cast);
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

	// Functor entry point
	void operator ()()
	{
		if (!m_mainScope) return;
		analyzeScope(m_mainScope);
	}
};