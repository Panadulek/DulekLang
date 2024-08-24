#pragma once
#include "AstElement.hpp"
#include "AstScope.hpp"
#include "VariableDecorator.hpp"
class AstVariableDecl : public AstElement
{
public:
	using ArrayDecorator = VariableDecorator::Array;
private:
	BasicTypes m_type;
	std::string_view m_id;
	int m_pointerLevel;
	std::optional<ArrayDecorator> m_array;
public:
	explicit AstVariableDecl(BasicTypes, std::string_view, AstScope*);
	BasicTypes getVarType();
	AstScope* getScope(){return ast_element_cast<AstScope>(getParent());}
	void setupArrayByString(std::string_view str)
	{
		m_array.emplace(str);
	}



};
