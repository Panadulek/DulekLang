#pragma once
#include "AstElement.hpp"
#include "BasicType.hpp"
#include "AstVariableDecl.hpp"
#include <optional>
#include <string_view>
class AstRef : public AstElement
{
	AstElement* m_ref;
public:
	AstRef(AstElement* ref) : m_ref(ref), AstElement("Reference", AstElement::ElementType::REFERENCE) {}
	std::optional<BasicTypes> getType()
	{
		if (AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(m_ref))
		{
			return decl->getVarType();
		}
		return std::nullopt;
	}
	std::string_view getName() { return m_ref->getName(); }
	AstElement* ref()
	{
		return m_ref;
	}
};