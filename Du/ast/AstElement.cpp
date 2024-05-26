#include "AstElement.hpp"

AstElement::AstElement(std::string_view name, ElementType astType) : m_name(name), m_astType(astType)
{}
std::string_view AstElement::getName() const { return m_name; }
const AstElement::ElementType AstElement::getAstType() { return m_astType; }
const bool AstElement::nameIsKeyword()
{
	return m_astType == ElementType::LOOP_EXPR || m_astType == ElementType::TYPE || m_astType == ElementType::POINTER_TYPE || m_astType == ElementType::CONST_NUMBER || m_astType == ElementType::CONDITION_EXPR;
}
