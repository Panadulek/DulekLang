#pragma once
#include "AstScope.hpp"
#include "AstBuilder.hpp"
#include <string_view>
#include <memory>
#include "AstExpr.hpp" 
#include <functional>
#include "AstVariableDecl.hpp"
#include "AstStatement.hpp"
#include "../Terminal/Terminal.hpp"
#include "AstCallFun.hpp"
#include "CastGraph.hpp"
#include "AstList.hpp"
#include "AstCast.hpp"

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

    static std::unique_ptr<AstExpr> toExpr(AstElement* el) {
        
        
        if (!el) return nullptr;

        if (auto* expr = ast_element_cast<AstExpr>(el)) {
            return std::unique_ptr<AstExpr>(expr);
        }
        return nullptr;
    }

	struct ExprFactor
	{
		AstExpr* createAddExpr(AstElement* left, AstElement* right)
		{
            AstNodes::BinaryExpr binOp { AstNodes::BinaryOpType::Addition, toExpr(left), toExpr(right) };
			return new AstExpr(std::move(binOp));
		}
		AstExpr* createSubExpr(AstElement* left, AstElement* right)
		{
            AstNodes::BinaryExpr binOp { AstNodes::BinaryOpType::Subtraction, toExpr(left), toExpr(right) };
			return new AstExpr(std::move(binOp));
		}
		AstExpr* createMulExpr(AstElement* left, AstElement* right)
		{
            AstNodes::BinaryExpr binOp { AstNodes::BinaryOpType::Multiplication, toExpr(left), toExpr(right) };
			return new AstExpr(std::move(binOp));
		}
		AstExpr* createDivExpr(AstElement* left, AstElement* right)
		{
            AstNodes::BinaryExpr binOp { AstNodes::BinaryOpType::Division, toExpr(left), toExpr(right) };
			return new AstExpr(std::move(binOp));
		}
		AstExpr* createUnsignedConst(uint64_t val)
		{
            AstNodes::LiteralExpr lit { val };
			return new AstExpr(std::move(lit));
		}
		AstExpr* createStrConst(std::string_view val)
		{
            AstNodes::LiteralExpr lit { std::string(val) };
			return new AstExpr(std::move(lit));
		}
		AstExpr* createBoolConst(bool val)
		{
            AstNodes::LiteralExpr lit { val };
			return new AstExpr(std::move(lit));
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
            
            AstNodes::VariableRefExpr varRef { std::string(name), element };
			AstExpr* expr = new AstExpr(std::move(varRef));
			expr->setValueCategory(ValueCategory::LValue);
			return expr;
		}

		AstExpr* createCast(AstElement* exprToCast, CastOp op)
		{
            AstNodes::CastExpr castExpr { op, toExpr(exprToCast) };
			return new AstExpr(std::move(castExpr));
		}
		AstExpr* createCmpExpr(AstElement* left, AstExpr::CMP_OPERATION op, AstElement* right)
		{
            AstNodes::CmpOpType newOp;
            switch(op)
            {
                case AstExpr::CMP_OPERATION::EQUAL: newOp = AstNodes::CmpOpType::Equal; break;
                case AstExpr::CMP_OPERATION::NOT_EQUAL: newOp = AstNodes::CmpOpType::NotEqual; break;
                case AstExpr::CMP_OPERATION::LESS_THAN: newOp = AstNodes::CmpOpType::LessThan; break;
                case AstExpr::CMP_OPERATION::GREATER_THAN: newOp = AstNodes::CmpOpType::GreaterThan; break;
                case AstExpr::CMP_OPERATION::LESS_OR_EQ: newOp = AstNodes::CmpOpType::LessOrEq; break;
                case AstExpr::CMP_OPERATION::GREATER_OR_EQ: newOp = AstNodes::CmpOpType::GreaterOrEq; break;
            }

            AstNodes::CmpExpr cmp { newOp, toExpr(left), toExpr(right) };
			return new AstExpr(std::move(cmp));
		}

		AstExpr* createCallFun(std::string_view funName, AstScope* beginContainer,  AstList* args)
		{
			AstElement* element = beginContainer->getElement(funName);
			AstExpr* res = nullptr;
			if (!element)
			{
				Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::DU001, funName);
			}
			if (AstScope* scope = ast_element_cast<AstScope>(element))
			{
				res = scope->accept(overloaded{
						[&](ScopeDecorator::Function& func) -> AstExpr*
						{
							 std::vector<std::unique_ptr<AstExpr>> funcArgs;
							if (auto* astArgs = dynamic_cast<AstArgs*>(args)) {
							for (auto& argPtr : astArgs->getArgs()) {
									funcArgs.push_back(toExpr(argPtr.release()));
								}
							}
							if (args) delete args;
							AstNodes::FunctionCallExpr call { std::string(funName), std::move(funcArgs) };
							return new AstExpr(std::move(call));
						},
						[&](auto&&) -> AstExpr*
						{
							return nullptr;
						}
					}
				);
			}
            return res;
		}

		AstExpr* createArrayIndexingOp(std::string_view varName, ArrayDecorator::Array& dims)
		{
			AstScope* scope = getActualScope();
			AstVariableDecl* element = ast_element_cast<AstVariableDecl>(scope->getElement(varName));
			
			if (element)
			{
                std::vector<std::unique_ptr<AstExpr>> indices;
                for(auto& dim : dims) {
                    if(dim) {
                         indices.push_back(toExpr(ast_element_cast<AstExpr>(dim->releaseExpr())));
                    }
                }

                // Create base variable reference
                AstNodes::VariableRefExpr varRef { std::string(varName), element };
                auto baseExpr = std::make_unique<AstExpr>(std::move(varRef));

                AstNodes::ArrayIndexingExpr arrIndex { std::move(baseExpr), std::move(indices) };
                
				AstExpr* expr = new AstExpr(std::move(arrIndex));
                expr->setValueCategory(ValueCategory::LValue);
                return expr;
			}
			else
				Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::DU001, varName);
			return nullptr;
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