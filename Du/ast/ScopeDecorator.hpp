#pragma once
#include <memory>;
#include "BasicType.hpp"
#include <vector>;
#include "AstElement.hpp"
#include "span"
#include "AstList.hpp"


namespace ScopeDecorator
{
	class Function
	{
	private:
		using _CONTAINER = std::vector<std::unique_ptr<AstElement>>;
		BasicTypes m_retType;
		_CONTAINER&& m_args;
		AstScope* m_ptrToScope;
	public:
		using CONTAINER = _CONTAINER;
		Function(BasicTypes retType, std::vector<std::unique_ptr<AstElement>>&& args, AstScope* scope) : m_retType(retType), m_args(std::move(args)), m_ptrToScope(scope)
		{}
		Function() = delete;
		Function(Function&) = delete;
		Function(Function&&) = delete;
		
		bool isProcedure() const;
		bool hasRetType() const;
		BasicTypes getRetType() const;
		std::span<std::unique_ptr<AstElement>> getArgs() const;
		std::size_t getArgsCounter() const;
		std::string_view getFunName() const;
		const AstScope* getFun() const;
	};
};
