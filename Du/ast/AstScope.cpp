#include "AstScope.hpp"
#include "algorithm"
#include <span>
#include <ranges>
#include <cassert>
AstScope::AstScope(std::string_view name, AstElement* parent) : AstElement(name, AstElement::ElementType::SCOPE), m_parent(parent)
{}

void AstScope::addElement(std::unique_ptr<AstElement>&& element)
{
	auto filteredView = m_elements | std::views::filter([](std::unique_ptr<AstElement>& element) { return !element->nameIsKeyword(); });
	if (!std::ranges::empty(filteredView))
	{
		if (std::ranges::none_of(filteredView, [&](std::unique_ptr<AstElement>& it) {return !it->getName().compare(element->getName());}))
		{
			m_elements.emplace_back(std::move(element));
		}
		else
		{
			assert(false && "Name was declared");
		}
	}
	else
		m_elements.emplace_back(std::move(element));
}

AstScope& AstScope::getGlobal()
{
	static AstScope globalScope("Global", nullptr);
	return globalScope;
}


AstScope::~AstScope() = default;