#include "AstCast.hpp"
#include "AstScope.hpp"
#include "AstVariableDecl.hpp"

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
#undef CAST_CONDITION