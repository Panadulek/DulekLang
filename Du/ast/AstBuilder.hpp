#pragma once
#include "AstScope.hpp"
#include "AstCast.hpp"
#include <cassert>
class AstBuilder
{
	AstScope* m_actualScope;
	AstScope* m_fileScope;

public:
	AstBuilder(std::string_view fname) : m_actualScope()
	{
		m_fileScope = AstScope::GlobalApi::getGlobalScopeForFile(fname);
	}
	AstElement* addElement(std::unique_ptr<AstElement>&& element)
	{
		if (element)
		{
			if (AstScope* castedScope = ast_unique_element_cast<AstScope>(element))
			{
				m_actualScope = reinterpret_cast<AstScope*>(m_actualScope->addElement(std::move(element)));
				return m_actualScope;
			}
			else
			{
				return m_actualScope->addElement(std::move(element));
	
			}
		}
	}
	AstScope* getActualScope()
	{
		return m_actualScope;
	}
	void exitScope()
	{
		m_actualScope = ast_element_cast<AstScope>(m_actualScope->getParent());
	}
	void beginScope(AstElement* scope)
	{
		if (AstScope* _s = ast_element_cast<AstScope>(scope))
		{
			m_actualScope = _s;
			return;
		}
		assert(0);
	}
	AstBuilder(AstBuilder&) = delete;
	AstBuilder(AstBuilder&&) = delete;
	~AstBuilder() = default;
};