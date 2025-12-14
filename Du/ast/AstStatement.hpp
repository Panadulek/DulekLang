#pragma once

#include "AstElement.hpp"
#include "AstExpr.hpp"
#include "AstVariableDecl.hpp"
class AstStatement : public AstElement
{
	AstElement* m_lhsRef = nullptr;
	std::unique_ptr<AstExpr> m_lhsExpr = nullptr;

	std::unique_ptr<AstExpr> m_rhs;
	enum class TYPE
	{
		EMPTY_STMT = 0,
		ASSIGN,
		RHS_STMT,
		RET_STMT,
		DECLARATION,
	};
	const TYPE m_stmtType;
public:
	using STMT_TYPE = TYPE;
	AstStatement() = delete;
	explicit AstStatement(std::unique_ptr<AstExpr> lhs, std::unique_ptr<AstExpr> rhs, STMT_TYPE type);
	explicit AstStatement(AstElement*, std::unique_ptr<AstExpr>, STMT_TYPE type);
	explicit AstStatement(AstVariableDecl*, std::unique_ptr<AstExpr>);
	const STMT_TYPE getStmtType() const { return m_stmtType; }
	AstExpr* rhs() { return m_rhs.get(); }
    std::unique_ptr<AstExpr> releaseRhs() { return std::move(m_rhs); }
	void setWrappedRhs(std::unique_ptr<AstExpr>&& expr) { m_rhs = std::move(expr); }
	AstElement* lhs() { return m_lhsExpr ? m_lhsExpr.get() : m_lhsRef; }
	
};