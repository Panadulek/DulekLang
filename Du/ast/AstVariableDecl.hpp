#pragma once
#include "AstElement.hpp"
#include "AstScope.hpp"
class AstVariableDecl : public AstElement
{

	BasicTypes m_type;
	std::string_view m_id;
	int m_pointerLevel;
public:
	explicit AstVariableDecl(BasicTypes, std::string_view, AstScope*);
	BasicTypes getVarType() { return m_type; }
	AstScope* getScope()
	{
		return ast_element_cast<AstScope>(getParent());
	}
};
