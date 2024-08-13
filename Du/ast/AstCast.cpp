
#include "AstCast.hpp"


class AstExpr;
class AstScope;
class AstVariableDecl;
class AstStatement;
class AstConst;
class AstRef;
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


template<>
AstExpr* ast_element_cast<AstExpr>(AstElement* source)
{

	if (CAST_CONDITION(source, AstElement::ElementType::EXPR))
		return reinterpret_cast<AstExpr*>(source);
	return nullptr;
}

template<>
AstStatement* ast_element_cast<AstStatement>(AstElement* source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::STATEMENT))
		return reinterpret_cast<AstStatement*>(source);
	return nullptr;
}

template<>
AstConst* ast_element_cast<AstConst>(AstElement* source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::CONST_))
		return reinterpret_cast<AstConst*>(source);
	return nullptr;
}
template<>
AstRef* ast_element_cast<AstRef>(AstElement* source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::REFERENCE))
		return reinterpret_cast<AstRef*>(source);
	return nullptr;
}
#undef CAST_CONDITION

