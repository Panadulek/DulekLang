#include "AstVariableDecl.hpp"
#include "BasicType.hpp"
#include "AstScope.hpp"
AstVariableDecl::AstVariableDecl(BasicTypes tname, std::string_view id, AstScope* parent) : AstElement(id, AstElement::ElementType::DECLARATION_VARIABLE, parent), m_type(tname), m_pointerLevel(0)
{

}