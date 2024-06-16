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

class AstScope final : public AstElement
{
	using Function = ScopeDecorator::Function;

	class Global;

	std::vector<std::unique_ptr<AstElement>> m_elements;
	AstElement* m_parent;
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



	void setUpFunction(BasicTypes retType, std::vector<std::unique_ptr<AstElement>>&& args)
	{
		m_function = std::make_unique<Function>(retType, std::move(args));
	}


public:
	AstScope() = delete;
	AstScope(AstScope&) = delete;
	AstScope(AstScope&&) = delete;
	explicit AstScope(std::string_view name, AstElement* parent);
	AstElement* addElement(std::unique_ptr<AstElement>&& element);
	std::span<std::unique_ptr<AstElement>> getElements();
	AstElement* getParent() const { return m_parent; }
	std::size_t getElementsCounter() const { return m_elements.size(); }
	struct GlobalApi
	{
		static bool addFile(std::string_view filename);
		static std::pair<bool, BasicTypes> isBasicType(std::string_view tname);
		static AstScope* getCurrentScope();
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