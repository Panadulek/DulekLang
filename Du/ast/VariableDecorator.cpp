#include "VariableDecorator.hpp"
#include "AstExpr.hpp"
ArrayDecorator::Dimension::Dimension(AstExpr* expr, bool owner) : m_dimension(expr), m_owner(owner)
{
	if (!expr)
		return;
	switch (expr->op())
	{
	case AstExpr::Operation::ConstValue:
		m_isDynamic = false;
		break;
	default:
		m_isDynamic = true;
		break;
	}
}

AstExpr* ArrayDecorator::Dimension::getExpr()
{
	return m_dimension.get();
}


AstExpr* ArrayDecorator::Dimension::releaseExpr()
{
	m_owner = false;
	return m_dimension.release();
}

ArrayDecorator::Dimension::~Dimension()
{
	if (!m_owner)
	{
		m_dimension.release();
	}
}
