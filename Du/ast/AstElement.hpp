#pragma once
#include <string>
#include <string_view>

class AstElement
{
public:
	enum class ElementType
	{
		CONST_NUMBER,
		TYPE,
		POINTER_TYPE,
		DECLARATION_VARIABLE,
		CONDITION_EXPR,
		LOOP_EXPR,
		SCOPE,
		EXPR,
		CALL_FUN_EXPR,
	};
private:
	std::string m_name;
	ElementType m_astType;
protected:
	AstElement(std::string_view, ElementType);
public:
	AstElement() = delete;
	AstElement(AstElement&) = delete;
	AstElement(AstElement&&) = delete;
	std::string_view getName() const ;
	const ElementType getAstType();
	const bool nameIsKeyword();
	virtual ~AstElement() = default;
};