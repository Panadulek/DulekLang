#pragma once

#include "AstElement.hpp"
#include "AstExpr.hpp"
#include "AstVariableDecl.hpp"
#include "AstControlBlock.h"
class AstStatement : public AstElement
{
	AstElement* m_lhsRef = nullptr;
	std::optional<AstControlBlock> m_controlBlock;
	std::unique_ptr<AstExpr> m_lhsExpr = nullptr;
	std::unique_ptr<AstExpr> m_rhs;
	enum class TYPE
	{
		EMPTY_STMT = 0,
		ASSIGN,
		RHS_STMT,
		RET_STMT,
		DECLARATION,
		CONDITION_BLOCK,
		LOOP_BLOCK,
	};
	const TYPE m_stmtType;
public:
	using STMT_TYPE = TYPE;
	AstStatement() = delete;
	explicit AstStatement(std::unique_ptr<AstExpr> lhs, std::unique_ptr<AstExpr> rhs, STMT_TYPE type, AstElement* parent = nullptr);
	explicit AstStatement(AstElement*, std::unique_ptr<AstExpr>, STMT_TYPE type, AstElement* parent = nullptr);
	explicit AstStatement(AstVariableDecl*, std::unique_ptr<AstExpr>, AstElement* parent = nullptr);
	const STMT_TYPE getStmtType() const { return m_stmtType; }
	AstExpr* rhs() { return m_rhs.get(); }
    std::unique_ptr<AstExpr> releaseRhs() { return std::move(m_rhs); }
	void setWrappedRhs(std::unique_ptr<AstExpr>&& expr) { m_rhs = std::move(expr); }
	AstElement* lhs() { return m_lhsExpr ? m_lhsExpr.get() : m_lhsRef; }
	bool isControlBlockStmt() const { return (m_stmtType == STMT_TYPE::CONDITION_BLOCK || m_stmtType == STMT_TYPE::LOOP_BLOCK) && m_controlBlock.has_value(); }
	std::optional<AstControlBlock>& getControlBlock() { return m_controlBlock; }
};