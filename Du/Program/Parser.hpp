#pragma once
#include <string_view>
#include <memory>
#include "../ast/AstScope.hpp"
#include "../ast/AstFactor.hpp"

class Parser final
{
	std::string_view m_currentFileName;
	std::size_t m_currentLine;
	struct Impl;
	std::unique_ptr<Impl> m_pimpl;
	bool& m_debugParser;
public:
	Parser() = delete;
	Parser(std::string_view filename, bool debug = false);
	void parse();
	~Parser();
};
