#include "AstFactor.hpp"
#include "AstBuildSystem.hpp"
#include "AstBuilder.hpp"
#include "ScopeDecorator.hpp"


std::unique_ptr<AstScope> AstFactory::ScopeFactor::createScope(std::string_view scopeName, AstElement* parent)
{
	return std::make_unique<AstScope>(scopeName, parent);
}

std::unique_ptr<AstScope> AstFactory::ScopeFactor::createFunction(std::string_view name, AstElement* parent, std::string_view retType, ScopeDecorator::Function::CONTAINER* args)
{
	auto isBuiltInType = AstScope::GlobalApi::isBasicType(retType);
	if (isBuiltInType.first)
	{
		std::unique_ptr<AstScope> newScope = createScope(name, parent);
		newScope->setUpFunction(isBuiltInType.second, args);
		return newScope;
	}
	return nullptr;
}

std::unique_ptr<AstScope> AstFactory::ScopeFactor::createFunction(std::string_view name, std::string_view retType, ScopeDecorator::Function::CONTAINER* args)
{
	return createFunction(name, AstBuildSystem::Instance().getBuilder().getActualScope(), retType, args);
}


