
#include <memory>;
#include "BasicType.hpp"
#include <vector>;
#include "AstElement.hpp"
#include "span"
namespace ScopeDecorator
{
	class Function
	{
	private:
		BasicTypes m_retType;
		std::vector<std::unique_ptr<AstElement>>&& m_args;
	public:
		Function(BasicTypes retType, std::vector<std::unique_ptr<AstElement>>&& args) : m_retType(retType), m_args(std::move(args))
		{}
		Function() = delete;
		Function(Function&) = delete;
		Function(Function&&) = delete;
		bool isProcedure() { return m_retType == BasicTypes::VOID_TYPE; }
		bool hasRetType() { return !isProcedure(); }
		BasicTypes getRetType() { return m_retType; }
		std::span<std::unique_ptr<AstElement>> getArgs() { return m_args; }
		std::size_t getArgsCounter() { return m_args.size(); }
	};
};
