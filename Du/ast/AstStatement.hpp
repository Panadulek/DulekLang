#pragma once

#include "AstElement.hpp"
#include "AstExpr.hpp"
#include "AstVariableDecl.hpp"
class AstStatement : public AstElement
{
	AstElement* m_lhs;
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
	explicit AstStatement(AstElement*, AstExpr*, STMT_TYPE type);
	explicit AstStatement(AstVariableDecl*, AstExpr*);
	const STMT_TYPE getStmtType() const { return m_stmtType; }
	AstExpr* rhs() { return m_rhs.get(); }
	AstElement* lhs() { return m_lhs; }
	~AstStatement() = default;
};