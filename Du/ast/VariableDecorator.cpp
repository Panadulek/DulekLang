#include "VariableDecorator.hpp"
#include "AstExpr.hpp"
#include "AstVisitor.hpp" 

struct IsDynamicVisitor {
    bool operator()(const AstNodes::LiteralExpr&) const { return false; }
    
    template<typename T>
    bool operator()(const T&) const { return true; }
};

ArrayDecorator::Dimension::Dimension(AstExpr* expr, bool owner) : m_dimension(expr), m_owner(owner)
{
	if (!expr)
		return;
    
    m_isDynamic = std::visit(IsDynamicVisitor{}, expr->getExpression());
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
