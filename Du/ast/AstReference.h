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
	AstRef(AstElement* ref) : m_ref(ref), AstElement("Reference", AstElement::ElementType::REFERENCE) 
	{}
	std::optional<BasicTypes> getType()
	{
		if (AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(m_ref))
		{
			auto type = decl->getVarType();
			if (type)
			{
				return type->getType();
			}
		}
		else if (AstScope* scope = ast_element_cast<AstScope>(m_ref))
		{
			if (ScopeDecorator::Function* fun = scope->getFunctionDecorator())
			{
				return fun->getRetType();
			}
		}
		return std::nullopt;
	}
	std::string_view getName() const override { return m_ref->getName(); }
	AstElement* ref()
	{
		return m_ref;
	}
};