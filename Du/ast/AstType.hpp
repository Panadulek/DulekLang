#pragma once
#include "BasicType.hpp"
#include "VariableDecorator.hpp"
#include <optional>
#include <string>
#include <unordered_set>
#include "AstScope.hpp"
#include <cassert>
class AstType
{
	using ArrayDecorator = VariableDecorator::Array;
	BasicTypes m_simpleType;
	std::unique_ptr<ArrayDecorator> m_arrayDims;
	class Cache //implement Cache for Basic Types in future
	{

	};
public:
	AstType(BasicTypes type) : m_simpleType(type), m_arrayDims(nullptr) {}
	void initArray(VariableDecorator::Array* arr)
	{
		if (!m_arrayDims)
			m_arrayDims.reset(arr);
		else
			assert(0);
	}
	std::optional<ArrayDecorator::const_iterator> beginArrayRange() const
	{
		if (!m_arrayDims)
			return std::nullopt;
		return m_arrayDims->begin();
	}

	std::optional<ArrayDecorator::const_iterator> endArrayRange() const
	{
		if (!m_arrayDims)
			return std::nullopt;
		return m_arrayDims->end();
	}

	bool isArray() const
	{
		return m_arrayDims != nullptr;
	}
	std::size_t getDimensionCounter() const
	{
		if (m_arrayDims)
			return m_arrayDims->size();
		return 0;
	}
	BasicTypes getType() const
	{
		return m_simpleType;
	}
	bool isBasicType() const
	{
		switch (m_simpleType)
		{
		case BasicTypes::I8:
		case BasicTypes::I16:
		case BasicTypes::I32:
		case BasicTypes::I64:
		case BasicTypes::U8:
		case BasicTypes::U16:
		case BasicTypes::U32:
		case BasicTypes::U64:
		case BasicTypes::F32:
		case BasicTypes::F64:
			return true;
		default:
			return false;
		}
	}
};