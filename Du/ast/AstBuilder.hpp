#pragma once
#include "AstScope.hpp"
#include "AstCast.hpp"

class AstBuilder
{
	AstScope* m_actualScope;
	AstScope* m_fileScope;
public:
	AstBuilder(std::string_view fname)
	{
		m_fileScope = AstScope::GlobalApi::getGlobalScopeForFile(fname);
		m_actualScope = m_fileScope;
	}
	void addElement(std::unique_ptr<AstElement>&& element)
	{
		if (element)
		{
			if (AstScope* castedScope = ast_unique_element_cast<AstScope>(element))
			{
				m_actualScope = reinterpret_cast<AstScope*>(m_actualScope->addElement(std::move(element)));
			}
			else
			{
				m_actualScope->addElement(std::move(element));
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

	AstBuilder(AstBuilder&) = delete;
	AstBuilder(AstBuilder&&) = delete;
	~AstBuilder() = default;
};