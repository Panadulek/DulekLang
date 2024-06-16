#pragma once
#include "AstElement.hpp"
#include "AstScope.hpp"
class AstVariableDecl : public AstElement
{
	using VariableType = BasicTypes;
	VariableType m_type;
	std::string_view m_id;
	int m_pointerLevel;
public:
	AstVariableDecl(std::string_view tname, std::string_view id);

};
