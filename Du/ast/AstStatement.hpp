#pragma once

#include "AstElement.hpp"
#include "AstExpr.hpp"
class AstStatement : public AstElement
{
	AstElement* m_lhs;
	AstExpr* m_rhs;
	enum class TYPE
	{
		EMPTY_STMT = 0,
		ASSIGN,
		RHS_STMT,
		RET_STMT,
	};
	const TYPE m_stmtType;
public:
	using STMT_TYPE = TYPE;
	AstStatement() = delete;
	AstStatement(AstElement*, AstExpr*, STMT_TYPE type);
	const STMT_TYPE getStmtType() const { return m_stmtType; }
	AstExpr* rhs() { return m_rhs; }
	AstElement* lhs() { return m_lhs; }
	~AstStatement() = default;
};