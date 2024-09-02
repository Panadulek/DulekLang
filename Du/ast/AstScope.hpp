#pragma once
#include "AstElement.hpp"
#include <string_view>
#include<vector>
#include <memory>
#include <functional>
#include <ranges>
#include <algorithm>
#include <stack>

#include "BasicType.hpp"
#include "ScopeDecorator.hpp"
#include "AstCast.hpp"

#include <optional>

class AstScope final : public AstElement
{
	using Function = ScopeDecorator::Function;

	class Global;

	std::vector<std::unique_ptr<AstElement>> m_elements;
	std::vector<std::unique_ptr<AstElement>> m_stmts;
	std::unique_ptr<Function> m_function;
	
	template<typename Container, typename Predicate>
	auto filterView(Container& container, Predicate predicate)
	{
		return container | std::views::filter(predicate);
	}
	
	template<typename View, typename Predicate>
	bool isNewElementUnique(View view, Predicate predicate)
	{
		return std::ranges::none_of(view, predicate);
	}
	template<typename View> bool isEmptyView(View view) { return std::ranges::empty(view); }
	template<typename View>
	bool isNewElementUniqueByName(View view, std::unique_ptr<AstElement>& element)
	{
		return isNewElementUnique(view, [&](std::unique_ptr<AstElement>& it) { return !it->getName().compare(element->getName()); });
	}
	
	auto getFilteredViewByNotKeywordName();
	
	static AstScope::Global& getGlobal();



	void setUpFunction(BasicTypes retType, Function::CONTAINER* args)
	{
		m_function = std::make_unique<Function>(retType, args, this);
	}


public:
	AstScope() = delete;
	AstScope(AstScope&) = delete;
	AstScope(AstScope&&) = delete;
	explicit AstScope(std::string_view name, AstElement* parent);
	AstElement* addElement(std::unique_ptr<AstElement>&& element);
	std::span<std::unique_ptr<AstElement>> getElements();
	std::span<std::unique_ptr<AstElement>> getStmts();
	AstElement* getParent() const { return AstElement::getParent(); }
	Function* getFunctionDecorator() const { return m_function.get(); }
	AstElement* getElement(std::string_view id)
	{
		for (auto& it : m_elements)
		{
			if (it->getName() == id)
				return it.get();
		}
		if (m_function &&  m_function->hasArgs())
		{
			auto args = m_function->getArgs();
			for (auto& it : args)
			{
				if (it->getName() == id)
					return it;
			}
		}
		AstElement* ret = nullptr;
		if (getParent())
		{
			AstScope* parentScope = ast_element_cast<AstScope>(getParent());
			return parentScope->getElement(id);
		}
		return nullptr;
			
	}
	std::size_t getElementsCounter() const { return m_elements.size(); }

	std::optional<BasicTypes> getScopeType()
	{
		std::optional<BasicTypes> ret;
		if (m_function)
		{
			if (m_function->hasRetType())
			{
				ret = std::make_optional(m_function->getRetType());
			}
			else
			{
				ret = std::make_optional(BasicTypes::VOID_TYPE);
			}
		}
		return ret;
	}

	struct GlobalApi
	{
		static bool addFile(std::string_view filename);
		static std::pair<bool, BasicTypes> isBasicType(std::string_view tname);
		static bool isTotalType(BasicTypes type);
		static AstScope* getCurrentGlobalScope();
		static AstScope* getGlobalScopeForFile(std::string_view filename);
	private: //function used only for unit tests project
		static void clearGlobalScopes();
		friend class AstScopeUnitTests;
	};
	~AstScope();
private:
	friend class AstScopeUnitTests;
	friend class AstFactory;
};