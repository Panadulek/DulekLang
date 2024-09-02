#include "AstVariableDecl.hpp"
#include "BasicType.hpp"
#include "AstScope.hpp"
#include "AstExpr.hpp"
AstVariableDecl::AstVariableDecl(BasicTypes tname, std::string_view id, AstScope* parent) : 
	AstElement(id, AstElement::ElementType::DECLARATION_VARIABLE, parent), m_type(tname), m_pointerLevel(0), m_array(std::nullopt)
																											
{}

BasicTypes AstVariableDecl::getVarType() { return m_type; }

bool AstVariableDecl::addDimension(AstExpr* dim)
{
	if (m_array.has_value())
	{
		m_array->emplace_back(std::make_unique<VariableDecorator::Dimension>(dim));
		return true;
	}
	return false;
}

