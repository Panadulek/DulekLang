#include "AstStatement.hpp"
#include "AstVariableDecl.hpp"

// Konstruktor dla wyrażeń jako LHS (przejmuje własność)
AstStatement::AstStatement(std::unique_ptr<AstExpr> lhs, std::unique_ptr<AstExpr> rhs, STMT_TYPE type, AstElement* parent)
	: m_lhsExpr(std::move(lhs)), m_rhs(std::move(rhs)), m_stmtType(type), AstElement("Statement", AstElement::ElementType::STATEMENT, parent)
{
	m_lhsRef = nullptr;
	if (type == STMT_TYPE::CONDITION_BLOCK)
	{
		m_controlBlock.emplace(AstControlBlock("control_block", AstControlBlock::type::CONDITION_BLOCK, parent));
	}
	else if (type == STMT_TYPE::LOOP_BLOCK)
	{
		m_controlBlock.emplace(AstControlBlock("control_block", AstControlBlock::type::LOOP_BLOCK, parent));
	}
}

// Konstruktor dla referencji jako LHS (nie przejmuje własności)
AstStatement::AstStatement(AstElement* lhs, std::unique_ptr<AstExpr> rhs, STMT_TYPE type, AstElement* parent) 
	: m_lhsRef(lhs), m_rhs(std::move(rhs)), m_stmtType(type), AstElement("Statement", AstElement::ElementType::STATEMENT, parent)
{
	m_lhsExpr = nullptr;
	if (type == STMT_TYPE::CONDITION_BLOCK)
	{
		m_controlBlock.emplace(AstControlBlock("control_block", AstControlBlock::type::CONDITION_BLOCK, parent));
	}
	else if(type == STMT_TYPE::LOOP_BLOCK)
	{
		m_controlBlock.emplace(AstControlBlock("control_block", AstControlBlock::type::LOOP_BLOCK, parent));
	}
}

// Konstruktor dla deklaracji (specyficzny typ, nie przejmuje własności LHS)
AstStatement::AstStatement(AstVariableDecl* lhs, std::unique_ptr<AstExpr> expr, AstElement* parent) 
	: m_lhsRef(lhs), m_rhs(std::move(expr)), m_stmtType(AstStatement::STMT_TYPE::DECLARATION), AstElement("Statement", AstElement::ElementType::STATEMENT, parent)
{
	m_lhsExpr = nullptr;
}