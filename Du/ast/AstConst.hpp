#pragma once
#include "AstExpr.hpp"
#include "BasicType.hpp"
#include <variant>
#include <string_view>
#include <variant>

#define generate_getter_variant(T, M) T get_##T##_value() {return std::get<T>(M);}

class AstConst : public AstElement
{
	using string_view = std::string_view;
	using String = std::string;
	BasicTypes m_type;
	std::variant<uint64_t, int64_t, double, std::string, bool> m_value;
public:
	AstConst(uint64_t uVal) :m_type(BasicTypes::U64), m_value(uVal), AstElement("const", AstElement::ElementType::CONST_)
	{}
	AstConst(std::string& strVal) : m_type(BasicTypes::STR), AstElement("const", AstElement::ElementType::CONST_)
	{
		m_value = strVal;
	}
	AstConst(bool bVal) : m_type(BasicTypes::BOOL), m_value(bVal), AstElement("const", AstElement::ElementType::CONST_)
	{}
	BasicTypes getType() { return m_type; }
	generate_getter_variant(uint64_t, m_value)
	generate_getter_variant(int64_t, m_value)
	generate_getter_variant(double, m_value)
	generate_getter_variant(String, m_value)
	generate_getter_variant(bool, m_value)

};