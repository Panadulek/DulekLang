#include "AstCast.hpp"
#include "AstScope.hpp"
#include "AstVariableDecl.hpp"
#include "AstExpr.hpp"
#include "AstStatement.hpp"
#include "AstConst.hpp"
#include "AstReference.h"
#define CAST_CONDITION(X, Y) X && X->getAstType() == Y
template<>
AstScope* ast_unique_element_cast<AstScope>(std::unique_ptr<AstElement>& source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::SCOPE))
		return reinterpret_cast<AstScope*>(source.get());
	return nullptr;
}

template<>
AstVariableDecl* ast_unique_element_cast<AstVariableDecl>(std::unique_ptr<AstElement>& source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::DECLARATION_VARIABLE))
		return reinterpret_cast<AstVariableDecl*>(source.get());
	return nullptr;
}


template<>
AstExpr* ast_unique_element_cast<AstExpr>(std::unique_ptr<AstElement>& source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::EXPR))
		return reinterpret_cast<AstExpr*>(source.get());
	return nullptr;
}

template<>
AstStatement* ast_unique_element_cast<AstStatement>(std::unique_ptr<AstElement>& source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::STATEMENT))
		return reinterpret_cast<AstStatement*>(source.get());
	return nullptr;
}
template<>
AstConst* ast_unique_element_cast<AstConst>(std::unique_ptr<AstElement>& source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::CONST_))
		return reinterpret_cast<AstConst*>(source.get());
	return nullptr;
}


template<>
AstRef* ast_unique_element_cast<AstRef>(std::unique_ptr<AstElement>& source)
{
	if (CAST_CONDITION(source, AstElement::ElementType::REFERENCE))
		return reinterpret_cast<AstRef*>(source.get());
	return nullptr;
}

#undef CAST_CONDITION