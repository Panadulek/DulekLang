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
#include "../Terminal/Terminal.hpp"
#include "AstCallFun.hpp"
#include "CastGraph.hpp"
extern AstScope* getActualScope();

class AstBuildSystem;
class AstFactory
{

	struct ScopeFactor
	{
		std::unique_ptr<AstScope> createScope(std::string_view scopeName, AstElement* parent);
		std::unique_ptr<AstScope> createFunction(std::string_view name, AstElement* parent, std::string_view retType, ScopeDecorator::Function::CONTAINER* args);
		std::unique_ptr<AstScope> createFunction(std::string_view name, std::string_view retType, ScopeDecorator::Function::CONTAINER* args);
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
		AstExpr* createUnsignedConst(uint64_t val)
		{
			return  new AstExpr(nullptr, AstExpr::Operation::ConstValue, new AstConst(val));
		}
		AstExpr* createStrConst(std::string_view val)
		{
			std::string str(val);
			return  new AstExpr(nullptr, AstExpr::Operation::ConstValue, new AstConst(str));
		}
		AstExpr* createRef(std::string_view name)
		{
			if (!name.data())
				return nullptr;
			AstScope* scope = getActualScope();
			AstElement* element = scope->getElement(name);
			if (!element)
			{
				Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::CodeList::DU001, name);
				return nullptr;
			}
			AstExpr* expr = new AstExpr(nullptr, AstExpr::Operation::Reference, new AstRef(element));
			expr->setValueCategory(ValueCategory::LValue);
			return expr;
		}

		AstExpr* createCast(AstElement* exprToCast, CastOp op)
		{
			return new AstExpr(exprToCast, AstExpr::Operation::Cast, op);
		}

		AstExpr* createCallFun(std::string_view funName, AstScope* beginContainer,  AstList* args)
		{
			AstElement* element = beginContainer->getElement(funName);
			if (!element)
			{
				Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::DU001, funName);
			}
			if (AstScope* scope = ast_element_cast<AstScope>(element))
			{
				if (scope->getFunctionDecorator())
				{
					AstElement* callFun = new AstCallFun(dynamic_cast<AstArgs*>(args), scope);
					return new AstExpr(callFun, AstExpr::Operation::Call_fun, args);
					
				}
				// TO_DO ERRORS
			}
		}

		AstExpr* createArrayIndexingOp(std::string_view varName, ArrayDecorator::Array& dims)
		{
			AstScope* scope = getActualScope();
			AstVariableDecl* element = ast_element_cast<AstVariableDecl>(scope->getElement(varName));
			AstExpr* expr = nullptr;
			if (element)
			{
				for (int i = dims.size() - 1; i >= 0; --i)
				{
					expr = new AstExpr(dims[i]->releaseExpr(), AstExpr::Operation::Arr_Indexing, expr);
				}
				if (expr)
					expr = new AstExpr(createRef(varName), AstExpr::Operation::Arr_Indexing, expr);

				if (expr)
					expr->setValueCategory(ValueCategory::LValue);
			}
			else
				Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::DU001, varName);
			return expr;
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

		std::unique_ptr<AstElement> createArray(std::string_view tname, std::string_view idname, ArrayDecorator::Array* dims, AstScope* parent)
		{
			if (!dims)
				return nullptr;
			auto variable = createVariable(tname, idname, parent);
			if (AstVariableDecl* decl = ast_unique_element_cast<AstVariableDecl>(variable))
			{
				AstType* type = decl->getVarType();
				if(type)
					type->initArray(dims);
				return variable;
			}
			else
				delete dims;
			return nullptr;
		}
	};


	struct StatementFactor
	{
		std::unique_ptr<AstElement> createAssigmentVariable(std::string_view lhs, AstElement* rhs, AstScope* currentScope)
		{

			if (currentScope)
			{
				AstElement* scopeMember = currentScope->getElement(lhs);
				if (!scopeMember)
				{
					Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::DU001, lhs);
				}
				else if (AstExpr* expr = ast_element_cast<AstExpr>(rhs))
				{
					return std::make_unique<AstStatement>(scopeMember, expr, AstStatement::STMT_TYPE::ASSIGN);
				}
			}
			return nullptr;
		}
		
		std::unique_ptr<AstElement> createAssigmentVariable(AstElement* expr, AstElement* rhs)
		{

			if (AstExpr* _expr = ast_element_cast<AstExpr>(rhs))
			{	
				return std::make_unique<AstStatement>(expr, _expr, AstStatement::STMT_TYPE::ASSIGN);
			}

			return nullptr;
		}

		std::unique_ptr<AstElement> createStmt(AstElement* rhs)
		{
			if (rhs)
			{
				if (AstExpr* expr = ast_element_cast<AstExpr>(rhs))
				{
					return std::make_unique<AstStatement>(nullptr, expr, AstStatement::STMT_TYPE::RHS_STMT);
				}
			}
			return nullptr;
		}

		std::unique_ptr<AstElement> createRetStmt(AstElement* rhs)
		{
			if (rhs)
			{
				if (AstExpr* expr = ast_element_cast<AstExpr>(rhs))
				{
					return std::make_unique<AstStatement>(nullptr, expr, AstStatement::STMT_TYPE::RET_STMT);
				}
			}
			return nullptr;
		}

		std::unique_ptr<AstElement> createDeclStmt(AstElement* rhs, AstExpr* expr = nullptr)
		{
			if (rhs)
			{
				if (AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(rhs))
				{
					return std::make_unique<AstStatement>(decl, expr);
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