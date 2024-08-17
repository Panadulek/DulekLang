#include "ScopeDecorator.hpp"
#include "ScopeDecorator.hpp"
#include "AstScope.hpp"

using Function = ScopeDecorator::Function;

Function::Function(BasicTypes retType, CONTAINER* args, AstScope* scope) : m_retType(retType), m_args(args), m_ptrToScope(scope)
{
}
bool Function::isProcedure() const { return m_retType == BasicTypes::VOID_TYPE; }
bool Function::hasRetType() const { return !isProcedure(); }
BasicTypes Function::getRetType() const { return m_retType; }
const bool Function::hasArgs() const { return m_args && m_args->size() > 0; }
std::span<AstElement*> Function::getArgs() const { return (*m_args.get()); }
std::size_t Function::getArgsCounter() const { return m_args->size(); }
std::string_view Function::getFunName() const {return m_ptrToScope->getName();}
const AstScope* Function::getFun() const {return m_ptrToScope;}