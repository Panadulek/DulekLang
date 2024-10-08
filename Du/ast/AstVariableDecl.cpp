#include "AstVariableDecl.hpp"
#include "BasicType.hpp"
#include "AstScope.hpp"
#include "AstExpr.hpp"
AstVariableDecl::AstVariableDecl(BasicTypes tname, std::string_view id, AstScope* parent) : 
	AstElement(id, AstElement::ElementType::DECLARATION_VARIABLE, parent), m_type(std::make_unique<AstType>(tname)), m_pointerLevel(0), m_array(nullptr)
{}

AstType* AstVariableDecl::getVarType() { return m_type.get(); }

