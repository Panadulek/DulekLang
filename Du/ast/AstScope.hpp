#pragma once
#include "AstElement.hpp"
#include <string_view>
#include<vector>
#include <memory>
class AstScope final : public AstElement
{
private:
	std::vector<std::unique_ptr<AstElement>> m_elements;
	AstElement* m_parent;
public:
	AstScope() = delete;
	AstScope(AstScope&) = delete;
	AstScope(AstScope&&) = delete;
	AstScope(std::string_view, AstElement*);
	static AstScope& getGlobal();
	void addElement(std::unique_ptr<AstElement>&& element);

	~AstScope();
};