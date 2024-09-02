#include "ScopeDecorator.hpp"
#include "ScopeDecorator.hpp"
#include "AstScope.hpp"
#include "AstType.hpp"
using Function = ScopeDecorator::Function;

Function::Function(BasicTypes retType, CONTAINER* args, AstScope* scope) : m_retType(std::make_unique<AstType>(retType)), m_args(args), m_ptrToScope(scope)
{
}
ScopeDecorator::Function::~Function()
{
	if (hasArgs())
	{
		for (auto& it : *m_args)
			delete it;
	}
}
bool Function::isProcedure() const
{
	if(m_retType)
		return m_retType->getType() == BasicTypes::VOID_TYPE;
	assert(0);
	return true;
}
bool Function::hasRetType() const { return !isProcedure(); }
std::optional<BasicTypes> Function::getRetType() const 
{
	if (m_retType)
		return m_retType->getType();
	return std::nullopt;
}
const bool Function::hasArgs() const { return m_args && m_args->size() > 0; }
std::span<AstElement*> Function::getArgs() const { return (*m_args.get()); }
std::size_t Function::getArgsCounter() const { return m_args->size(); }
std::string_view Function::getFunName() const {return m_ptrToScope->getName();}
const AstScope* Function::getFun() const {return m_ptrToScope;}