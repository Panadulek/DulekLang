#pragma once
#include <string_view>
#include <memory>
class Parser final
{
	std::string_view m_currentFileName;
	std::size_t m_currentLine;

	struct Impl;

	std::unique_ptr<Impl> m_pimpl;

public:
	Parser() = delete;
	Parser(std::string_view filename);
	void parse();
	~Parser();
};
