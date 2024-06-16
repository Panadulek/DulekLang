#include "AstVariableDecl.hpp"
#include "BasicType.hpp"
AstVariableDecl::AstVariableDecl(std::string_view tname, std::string_view id) : AstElement(tname, AstElement::ElementType::DECLARATION_VARIABLE), m_type(BasicTypes::VOID_TYPE), m_pointerLevel(0)
{
	auto isBasicType = AstScope::GlobalApi::isBasicType(tname);
}