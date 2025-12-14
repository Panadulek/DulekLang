#include "AstStatement.hpp"
#include "AstVariableDecl.hpp"

// Konstruktor dla wyrażeń jako LHS (przejmuje własność)
AstStatement::AstStatement(std::unique_ptr<AstExpr> lhs, std::unique_ptr<AstExpr> rhs, STMT_TYPE type)
	: m_lhsExpr(std::move(lhs)), m_rhs(std::move(rhs)), m_stmtType(type), AstElement("Statement", AstElement::ElementType::STATEMENT)
{
	m_lhsRef = nullptr;
}

// Konstruktor dla referencji jako LHS (nie przejmuje własności)
AstStatement::AstStatement(AstElement* lhs, std::unique_ptr<AstExpr> rhs, STMT_TYPE type) 
	: m_lhsRef(lhs), m_rhs(std::move(rhs)), m_stmtType(type), AstElement("Statement", AstElement::ElementType::STATEMENT)
{
	m_lhsExpr = nullptr;
}

// Konstruktor dla deklaracji (specyficzny typ, nie przejmuje własności LHS)
AstStatement::AstStatement(AstVariableDecl* lhs, std::unique_ptr<AstExpr> expr) 
	: m_lhsRef(lhs), m_rhs(std::move(expr)), m_stmtType(AstStatement::STMT_TYPE::DECLARATION), AstElement("Statement", AstElement::ElementType::STATEMENT)
{
	m_lhsExpr = nullptr;
}