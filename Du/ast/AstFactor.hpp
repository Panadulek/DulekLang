#pragma once
#include "AstScope.hpp"
#include "AstBuilder.hpp"
#include <string_view>
#include <memory>
#include "AstFactor.hpp"
#include "AstExpr.hpp"
#include <functional>
#include "AstVariableDecl.hpp"
#include "AstStatement.hpp"
#include "AstConst.hpp"
#include "AstReference.h"

extern AstScope* getActualScope();

class AstBuildSystem;
class AstFactory
{

	struct ScopeFactor
	{
		std::unique_ptr<AstScope> createScope(std::string_view scopeName, AstElement* parent);
		std::unique_ptr<AstScope> createFunction(std::string_view name, AstElement* parent, std::string_view retType, std::vector<std::unique_ptr<AstElement>>& args);
		std::unique_ptr<AstScope> createFunction(std::string_view name, std::string_view retType, std::vector<std::unique_ptr<AstElement>>& args);
	};


	struct ExprFactor
	{
		AstExpr* createAddExpr(AstElement* left, AstElement* right)
		{
			return new AstExpr(left, AstExpr::Operation::Addition, right);
		}
		AstExpr* createSubExpr(AstElement* left, AstElement* right)
		{
			return new AstExpr (left, AstExpr::Operation::Subtraction, right);
		}
		AstExpr* createMulExpr(AstElement* left, AstElement* right)
		{
			return new AstExpr(left, AstExpr::Operation::Multiplication, right);
		}
		AstExpr* createDivExpr(AstElement* left, AstElement* right)
		{
			return new AstExpr(left, AstExpr::Operation::Division, right);
		}
		AstConst* createUnsignedConst(uint64_t val)
		{
			return new AstConst(val);
		}
	};
	struct VariableFactor
	{
		std::unique_ptr<AstElement> createVariable(std::string_view tname, std::string_view idname, AstScope* parent)
		{
			if (!idname.data())
				idname = "";
			auto isBuiltInType = AstScope::GlobalApi::isBasicType(tname);
			return std::make_unique<AstVariableDecl>(isBuiltInType.second, idname, parent);
		}

		AstRef* createRef(std::string_view name)
		{
			if(!name.data())
				return nullptr;
			AstScope* scope = getActualScope();
			AstElement* element = scope->getElement(name);
			return new AstRef(element);
		}

	};


	struct StatementFactor
	{
		std::unique_ptr<AstElement> createAssigmentVariable(std::string_view lhs, AstElement* rhs, AstScope* currentScope)
		{

			if (currentScope)
			{
				AstElement* scopeMember = currentScope->getElement(lhs);
				if (AstExpr* expr = ast_element_cast<AstExpr>(rhs))
				{
					return std::make_unique<AstStatement>(scopeMember, expr, AstStatement::STMT_TYPE::ASSIGN);
				}
			}
			return nullptr;
		}
	};

	std::unique_ptr<ScopeFactor> m_scopeFactor;
	std::unique_ptr<VariableFactor> m_varFactor;
	std::unique_ptr<ExprFactor> m_exprFactor;
	std::unique_ptr<StatementFactor> m_stmtFactor;
public:
	AstFactory() : m_scopeFactor(std::make_unique<ScopeFactor>()), m_varFactor(std::make_unique<VariableFactor>()), m_exprFactor(std::make_unique<ExprFactor>()), 
		m_stmtFactor(std::make_unique<StatementFactor>())
	{}
	AstFactory(AstFactory& factory) = delete;
	AstFactory(AstFactory&& factory) = delete;
	ScopeFactor& scopeFactor()
	{ 
		return *m_scopeFactor; 
	}
	ExprFactor& exprFactor()
	{
		return *m_exprFactor;
	}
	VariableFactor& varFactor()
	{
		return *m_varFactor;
	}
	StatementFactor& stmtFactor()
	{
		return *m_stmtFactor;
	}
	~AstFactory() = default;
};