#pragma once
#include "AstElement.hpp"
#include <memory>
#include <optional>
#include <functional>
#include "AstCast.hpp"
#include "VariableDecorator.hpp"
#include "AstType.hpp"
#include "CastGraph.hpp"
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
			Arr_Indexing,
			Cast
		};
		using IndexingArray = ArrayDecorator::Array;
private:
	bool isDeleteable(AstElement* toDelete)
	{
		if (!toDelete)
			return false;
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
	std::optional<CastOp> m_castOp;
	std::unique_ptr<AstType> m_type;
	std::unique_ptr<IndexingArray> createArrayFromIndexingOp();
public:

	explicit AstExpr(AstElement* left, Operation op, AstElement* right) : m_left(left), m_op(op), m_right(right), m_type(nullptr), AstElement("expr", AstElement::ElementType::EXPR)
	{}
	explicit AstExpr(AstElement* left, Operation op) : m_left(left), m_op(op), m_right(nullptr), m_type(nullptr), AstElement("expr", AstElement::ElementType::EXPR)
	{}
	explicit AstExpr(AstElement* left, Operation op, CastOp castOp) : m_left(left), m_op(op), m_right(nullptr), m_type(nullptr), m_castOp(castOp), AstElement("expr", AstElement::ElementType::EXPR)
	{}

	Operation op() const { return m_op; }
	
	AstElement* left() const { return m_left; }
	AstElement* right() const { return m_right; }
	
	void left(AstElement* left) { m_left = left; }
	void right(AstElement* right) { m_right = right; }

	void setType(AstType* type) { m_type.reset(type); }
	AstType* getType() const { return m_type.get(); }
	const bool isBinaryOp()
	{
		switch (m_op)
		{
		case Operation::Addition:
		case Operation::Subtraction:
		case Operation::Multiplication:
		case Operation::Division:
		case Operation::Arr_Indexing:
			return true;
		default:
			return false;
		}
	}

	const bool isCastExpr() const
	{
		return m_op == Operation::Cast && m_castOp.has_value();
	}
	const std::optional<CastOp> getCastOp() const
	{
		return m_castOp;
	}

	static std::unique_ptr<IndexingArray> transformExprToDimsArray(AstExpr* expr)
	{
		if (!expr || expr->op() != AstExpr::Operation::Arr_Indexing)
			return nullptr;
		auto ret = expr->createArrayFromIndexingOp();
		return ret;
	}

	virtual ~AstExpr()
	{
		if (isDeleteable(m_left))
			delete m_left;
		if (isDeleteable(m_right))
			delete m_right;
	}
};