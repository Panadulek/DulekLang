#include "VariableDecorator.hpp"
#include "AstExpr.hpp"
ArrayDecorator::Dimension::Dimension(AstExpr* expr) : m_dimension(expr)
{
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
	return m_dimension.release();
}

