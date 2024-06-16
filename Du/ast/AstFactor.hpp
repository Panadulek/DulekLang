#pragma once
#include "AstScope.hpp"
#include "AstBuilder.hpp"
#include <string_view>
#include <memory>

class AstFactory
{

	struct ScopeFactor
	{
		std::unique_ptr<AstScope> createScope(std::string_view scopeName, AstElement* parent)
		{
			return std::make_unique<AstScope>(scopeName, parent);
		}
		std::unique_ptr<AstScope> createFunction(std::string_view name, AstElement* parent, BasicTypes retType, std::vector<std::unique_ptr<AstElement>>& args)
		{
			std::unique_ptr<AstScope> newScope = createScope(name, parent);
			newScope->m_function = std::make_unique<ScopeDecorator::Function>(retType, std::move(args));
			return newScope;
		}
	};

	struct VariableFactor
	{

	};

	std::unique_ptr<ScopeFactor> m_scopeFactor;
	std::unique_ptr<VariableFactor> m_varFactor;

public:
	AstFactory() : m_scopeFactor(std::make_unique<ScopeFactor>()), m_varFactor(std::make_unique<VariableFactor>())
	{}
	AstFactory(AstFactory& factory) = delete;
	AstFactory(AstFactory&& factory) = delete;
	std::unique_ptr<AstScope> createFunction(std::string_view name, AstElement* parent, BasicTypes retType, std::vector<std::unique_ptr<AstElement>>& args) 
	{ 
		return m_scopeFactor->createFunction(name, parent, retType, args); 
	}
	std::unique_ptr<AstScope> createScope(std::string_view name, AstElement* parent)
	{
		return m_scopeFactor->createScope(name, parent);
	}

	~AstFactory() = default;
};