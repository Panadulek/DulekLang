#include "AstStatement.hpp"
#include "AstVariableDecl.hpp"
AstStatement::AstStatement(AstElement* lhs, AstExpr* rhs, STMT_TYPE type) : m_lhs(lhs), m_rhs(rhs), m_stmtType(type), AstElement("Statement", AstElement::ElementType::STATEMENT)
{}

AstStatement::AstStatement(AstVariableDecl* lhs, AstExpr* expr) : m_lhs(lhs), m_rhs(expr), m_stmtType(AstStatement::STMT_TYPE::DECLARATION), AstElement("Statement", AstElement::ElementType::STATEMENT)
{}