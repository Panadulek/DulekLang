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
#include <unordered_map>
#include <optional>
#include <variant>
#include "AstVisitor.hpp"

class AstScope final : public AstElement
{
	using Function = ScopeDecorator::Function;
	using ScopeVariant = std::variant<std::monostate, Function>;
	class Global;

	std::vector<std::unique_ptr<AstElement>> m_elements;
	std::vector<std::unique_ptr<AstElement>> m_stmts;
	std::unordered_map<std::string_view, AstElement*> m_lookupMap;	
	ScopeVariant m_scopeDecorator;

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
	
	static Global& getGlobal();



	void setUpFunction(BasicTypes retType, Function::CONTAINER* args)
	{
		m_scopeDecorator = Function(retType, args, this);
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

	template<typename Visitor>
	decltype(auto) accept(Visitor&& visitor)
	{
		return std::visit(std::forward<Visitor>(visitor), m_scopeDecorator);
	}

	AstElement* getElement(std::string_view id)
	{
		for (auto& it : m_elements)
		{
			if (it->getName() == id)
				return it.get();
		}

		AstElement* res = std::visit(
			overloaded{
				[&](ScopeDecorator::Function& func) -> AstElement*
				{
					if (func.hasArgs())
					{
						auto args = func.getArgs();
						for (auto& it : args)
						{
							if (it->getName() == id)
								return it;
						}
					}
					return nullptr; 
				},

			[&](auto&&) -> AstElement* {
				return nullptr;
			}
			}, m_scopeDecorator);

		if (res)
			return res;

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
		std::optional<BasicTypes> ret = std::visit(
			overloaded{
				[&](ScopeDecorator::Function& func) -> std::optional<BasicTypes>
				{
					return func.getRetType();
				},
				[&](auto&&) -> std::optional<BasicTypes>
				{
					return std::nullopt;
				}
			}, m_scopeDecorator);


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