#pragma once
#include <memory>;
#include "BasicType.hpp"
#include <vector>;
#include "AstElement.hpp"
#include "span"
#include "AstList.hpp"

class AstType;
namespace ScopeDecorator
{
	class Function
	{
	private:
		using _CONTAINER = std::vector<AstElement*>;
		std::unique_ptr<AstType> m_retType;
		std::unique_ptr<_CONTAINER> m_args;
		AstScope* m_ptrToScope;
	public:
		using CONTAINER = _CONTAINER;
		Function(BasicTypes retType, CONTAINER* args, AstScope* scope);
		Function() = delete;
		Function(Function&) = delete;
		Function(Function&&) = delete;
		~Function();
		bool isProcedure() const;
		bool hasRetType() const;
		std::optional<BasicTypes> getRetType() const;
		const bool hasArgs() const;
		std::span<AstElement*> getArgs() const;
		std::size_t getArgsCounter() const;
		std::string_view getFunName() const;
		const AstScope* getFun() const;
	};
};
