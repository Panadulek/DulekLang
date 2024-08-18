#include "AstVariableDecl.hpp"
#include "BasicType.hpp"
#include "AstScope.hpp"
AstVariableDecl::AstVariableDecl(BasicTypes tname, std::string_view id, AstScope* parent) : AstElement(id, AstElement::ElementType::DECLARATION_VARIABLE, parent), m_type(tname), m_pointerLevel(0), m_counter(0)
{

}

inline BasicTypes AstVariableDecl::getVarType() { return m_type; }

