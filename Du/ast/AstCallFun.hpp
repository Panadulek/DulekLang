#pragma once
#include "AstElement.hpp"
#include "AstList.hpp"
#include "ScopeDecorator.hpp"
#include "string_view"
#include "AstScope.hpp"
class AstCallFun : public AstElement
{
	std::unique_ptr<AstArgs> m_args;
	using Function = ScopeDecorator::Function;
	Function* m_fun;
	bool m_argsResolved;
	bool resolve();
public:
	std::size_t getArgsSize()
	{
		return m_args ? m_args->getSize() : 0;
	}
	explicit AstCallFun(AstArgs* args, Function* fun) : m_args(args), m_fun(fun), m_argsResolved(false), AstElement("CallFun", AstElement::ElementType::CALL_FUN)
	{
		m_argsResolved = resolve();
	}
	explicit AstCallFun(AstArgs* args, AstScope* scope) : AstCallFun(args, scope->getFunctionDecorator()) 
	{}
	const AstScope* getScope() const { return m_fun->getFun(); }
	std::span<std::unique_ptr<AstElement>> getArgs() { return m_args->getArgs(); }
	std::string_view getFunName() {m_fun->getFunName();}
	
};