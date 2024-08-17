#pragma once
#include "AstElement.hpp"
#include <memory>
#include <optional>
#include <functional>
#include "AstCast.hpp"
class AstExpr : public AstElement
{
public:
		enum class Operation : uint8_t
		{
			Addition = 0,
			Multiplication,
			Division,
			Subtraction,
			Unary_negation,
			ConstValue,
			Reference,
			Call_fun,
		};
private:
	bool isDeleteable(AstElement* toDelete)
	{
		auto type = toDelete->getAstType();
		switch (type)
		{
		case AstElement::ElementType::REFERENCE:
		case AstElement::ElementType::CONST_:
		case AstElement::ElementType::EXPR:
		case AstElement::ElementType::CALL_FUN:
			return true;
		default:
			return false;
		}
	}
	AstElement* m_left;
	AstElement* m_right;
	Operation m_op;
public:
	explicit AstExpr(AstElement* left, Operation op, AstElement* right) : m_left(left), m_op(op), m_right(right), AstElement("expr", AstElement::ElementType::EXPR)
	{}
	explicit AstExpr(AstElement* left, Operation op) : m_left(left), m_op(op), m_right(nullptr), AstElement("expr", AstElement::ElementType::EXPR)
	{}
	Operation op() const { return m_op; }
	AstElement* left() { return m_left; }
	AstElement* right() { return m_right; }
	~AstExpr()
	{
		if (isDeleteable(m_left))
			delete m_left;
		if (isDeleteable(m_right))
			delete m_right;
	}
};





