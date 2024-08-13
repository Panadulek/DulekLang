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
	BasicTypes m_type;
	std::variant<uint64_t, int64_t, double, std::string_view> m_value;
public:
	AstConst(uint64_t uVal) :m_type(BasicTypes::U64), AstElement("const", AstElement::ElementType::CONST_)
	{
		m_value = uVal;
	}
	BasicTypes getType() { return m_type; }
	generate_getter_variant(uint64_t, m_value)
	generate_getter_variant(int64_t, m_value)
	generate_getter_variant(double, m_value)
	generate_getter_variant(string_view, m_value)

};