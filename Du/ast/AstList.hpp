#pragma once
#include <list>
#include "AstElement.hpp"
#include "AstExpr.hpp"
#include "AstConst.hpp"
class AstList : public AstElement
{
protected:
	std::vector<std::unique_ptr<AstElement>> m_list;
	AstList() : AstElement("ast_list", AstElement::ElementType::AST_LIST)
	{}
public:
	virtual void push(AstElement*) = 0;
	virtual AstElement* getFront() = 0;
	std::size_t getSize()
	{
		return m_list.size();
	}
	virtual ~AstList() = default;
};


class AstArgs : public AstList
{
	decltype(m_list.begin()) m_curr;
public:
	AstArgs() : AstList() 
	{
		m_curr = m_list.end();
	}
	virtual void push(AstElement* newObject) override
	{
		if( auto ptr = ast_element_cast<AstExpr>(newObject))
			m_list.emplace_back(ptr);
		else if(auto ptr = ast_element_cast<AstConst>(newObject))
			m_list.emplace_back(ptr);
	}
	virtual AstElement* getFront() override
	{
		if (isEnd())
			return nullptr;
		AstElement* ret = (*m_curr).get();
		m_curr++;
		return ret;
	}
	bool isEnd()
	{
		return m_list.end() == m_curr;
	}
	void goToBegin()
	{
		m_curr = m_list.begin();
	}
	auto& getArgs()
	{
		return m_list;
	}
	virtual ~AstArgs()
	{

	}
};


