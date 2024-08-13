#include "AstStatement.hpp"

AstStatement::AstStatement(AstElement* lhs, AstExpr* rhs, STMT_TYPE type) : m_lhs(lhs), m_rhs(rhs), m_stmtType(type), AstElement("Statement", AstElement::ElementType::STATEMENT)
{}