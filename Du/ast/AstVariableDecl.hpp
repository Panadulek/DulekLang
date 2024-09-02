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
	std::unique_ptr<ArrayDecorator> m_array;
public:
	explicit AstVariableDecl(BasicTypes, std::string_view, AstScope*);
	BasicTypes getVarType();
	AstScope* getScope(){return ast_element_cast<AstScope>(getParent());}
	void initArrayDecorator(ArrayDecorator* decorator)
	{
		m_array.reset(decorator);
	}

	std::optional<ArrayDecorator::const_iterator> beginArrayRange()
	{
		if (!m_array)
			return std::nullopt;
		return m_array->begin();
	}

	std::optional<ArrayDecorator::const_iterator> endArrayRange()
	{
		if (!m_array)
			return std::nullopt;
		return m_array->end();
	}

	bool isArray()
	{
		return m_array != nullptr;
	}

	std::size_t getDimensionCounter()
	{
		if (m_array)
			return m_array->size();
		return 0;
	}

};
