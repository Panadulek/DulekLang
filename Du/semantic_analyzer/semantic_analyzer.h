#pragma once
#include "../ast/AstScope.hpp"
#include <string_view>
#include <queue>
#include "../ast/AstExpr.hpp"
#include <span>
#include "../ast/CastGraph.hpp"
#include <optional>
#include "type_checker.h"
#include <memory>
class SemanticAnalyzer
{
	
	std::queue<AstScope*> m_scopes;
public:
	SemanticAnalyzer(std::string_view filename) 
	{
		m_scopes.push(AstScope::GlobalApi::getGlobalScopeForFile(filename));
	}

	void analyzie()
	{
		std::vector<TypeChecker> m_checker;
		while (!m_scopes.empty())
		{
			TypeChecker& typeChecker = m_checker.emplace_back(TypeChecker(m_scopes.front()));
			typeChecker();
			m_scopes.pop();
		}
	}

};