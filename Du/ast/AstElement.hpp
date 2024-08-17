#pragma once
#include <string>
#include <string_view>
#include <functional>
#include <array>
class AstScope;
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
	};
private:
	std::string m_name;
	ElementType m_astType;
	AstElement* m_parent;
protected:
	AstElement(std::string_view, ElementType, AstElement* parent);
	AstElement(std::string_view, ElementType);

public:
	AstElement() = delete;
	AstElement(AstElement&) = delete;
	AstElement(AstElement&&) = delete;
	std::string_view getName() const ;
	const ElementType getAstType();
	AstElement* getParent() const { return m_parent; }
	const bool nameIsKeyword();
	virtual ~AstElement() = default;
};

