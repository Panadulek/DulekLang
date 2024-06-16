
#include "AstCast.hpp"
#include "AstScope.hpp"
#include "AstVariableDecl.hpp"
#define CAST_CONDITION(X, Y) X && X->getAstType() == Y
template<>
AstScope* ast_element_cast<AstScope>(AstElement* source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::SCOPE))
		return reinterpret_cast<AstScope*>(source);
	return nullptr;
}

template<>
AstVariableDecl* ast_element_cast<AstVariableDecl>(AstElement* source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::DECLARATION_VARIABLE))
		return reinterpret_cast<AstVariableDecl*>(source);
	return nullptr;
}





#undef CAST_CONDITION

