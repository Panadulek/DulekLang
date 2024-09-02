#pragma once
#include "AstElement.hpp"
#include "AstScope.hpp"
#include "VariableDecorator.hpp"
class AstExpr;
class AstVariableDecl : public AstElement
{
public:
	using ArrayDecorator = VariableDecorator::Array;
private:
	BasicTypes m_type;
	std::string_view m_id;
	int m_pointerLevel;
	std::optional<ArrayDecorator> m_array;
public:
	explicit AstVariableDecl(BasicTypes, std::string_view, AstScope*);
	BasicTypes getVarType();
	AstScope* getScope(){return ast_element_cast<AstScope>(getParent());}
	void initArrayDecorator()
	{
		m_array = ArrayDecorator();
	}
	bool addDimension(AstExpr* dim);

	std::optional<ArrayDecorator::const_iterator> beginArrayRange()
	{
		if (!m_array.has_value())
			return std::nullopt;
		return m_array->begin();
	}

	std::optional<ArrayDecorator::const_iterator> endArrayRange()
	{
		if (!m_array.has_value())
			return std::nullopt;
		return m_array->end();
	}

	bool isArray()
	{
		return m_array.has_value();
	}

	std::size_t getDimensionCounter()
	{
		if (m_array.has_value())
			return m_array->size();
		return 0;
	}

};
