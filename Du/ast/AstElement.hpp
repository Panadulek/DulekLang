#pragma once
#include <string>
#include <string_view>
#include <functional>
#include <array>
class AstScope;

enum class ValueCategory : uint8_t {
	RValue = 0, // Wartość tymczasowa (nie ma stałego adresu)
	LValue      // Wartość-lokator (posiada adres w pamięci, można do niej przypisać)
};

class AstElement
{
public:
	enum class ElementType : uint8_t
	{
		CONST_,
		TYPE,
		POINTER_TYPE,
		DECLARATION_VARIABLE,
		CONDITION_EXPR,
		LOOP_EXPR,
		SCOPE,
		EXPR,
		STATEMENT,
		REFERENCE,
		AST_LIST,
		CALL_FUN,
		CONTROL_BLOCK,
	};
private:
	std::string m_name;
	ElementType m_astType;
	AstElement* m_parent;
protected:
	ValueCategory m_value_category = ValueCategory::RValue;
	AstElement(std::string_view, ElementType, AstElement* parent);
	AstElement(std::string_view, ElementType);

public:
	AstElement() = delete;
	AstElement(AstElement&) = delete;
	AstElement(AstElement&&) = delete;
	virtual std::string_view getName() const ;
	const ElementType getAstType();
	AstElement* getParent() const { return m_parent; }
	void setParent(AstElement* parent) { m_parent = parent; }
	const bool nameIsKeyword();
	void setValueCategory(ValueCategory category) { m_value_category = category; }
	ValueCategory getValueCategory() const { return m_value_category; }
	bool isLValue() const { return m_value_category == ValueCategory::LValue; }
	virtual ~AstElement() = default;
};

