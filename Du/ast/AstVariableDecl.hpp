#pragma once
#include "AstElement.hpp"
#include "AstScope.hpp"
#include "VariableDecorator.hpp"
#include "AstType.hpp"
class AstExpr;
class AstVariableDecl : public AstElement
{
public:
	using ArrayDecorator = ArrayDecorator::Array;
private:
	std::unique_ptr<AstType> m_type;
	std::string_view m_id;
	int m_pointerLevel;
	std::unique_ptr<ArrayDecorator> m_array;
public:
	explicit AstVariableDecl(BasicTypes, std::string_view, AstScope*);
	AstType* getVarType();
	AstScope* getScope(){return ast_element_cast<AstScope>(getParent());}
};
