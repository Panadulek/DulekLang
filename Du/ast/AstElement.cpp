#include "AstElement.hpp"
#include "AstScope.hpp"
AstElement::AstElement(std::string_view name, ElementType astType, AstElement* parent) : m_name(name), m_astType(astType), m_parent(parent)
{}
AstElement::AstElement(std::string_view name, ElementType astType) : m_name(name), m_astType(astType), m_parent(nullptr)
{}
std::string_view AstElement::getName() const { return m_name; }
const AstElement::ElementType AstElement::getAstType() { return m_astType; }
const bool AstElement::nameIsKeyword()
{
	return m_astType == ElementType::LOOP_EXPR ||   m_astType == ElementType::CONDITION_EXPR || m_astType == ElementType::STATEMENT;
}