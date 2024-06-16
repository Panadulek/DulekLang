#include "Parser.hpp"
#include <cstdio>
#include <cstdint>
#include "../ast/AstCast.hpp"
#include "../ast/AstBuildSystem.hpp"

extern FILE* yyin;
extern int yyparse();
struct Parser::Impl
{
	
	Impl() = default;
	enum class ParserRetCodes : uint8_t
	{
		OK = 0,
		SYNTAX_ERROR = 1,
		MISSING_MEMORY = 2
	};
	bool openFile(std::string_view filename)
	{
		yyin = fopen(filename.data(), "r+");
		if (yyin)

			return true;
		return false;
	}
	ParserRetCodes parse()
	{
		return static_cast<ParserRetCodes>(yyparse());
	}
	~Impl() = default;

};


Parser::Parser(std::string_view filename) : m_currentFileName(filename), m_currentLine(0), m_pimpl(std::make_unique<Impl>())
{

}

void Parser::parse()
{
	if (m_currentFileName.empty())
		return;
	const bool isOpenFile = m_pimpl->openFile(m_currentFileName);
	Impl::ParserRetCodes ret = Impl::ParserRetCodes::OK;
	if (isOpenFile)
	{
		AstScope::GlobalApi::addFile(m_currentFileName);
		AstBuildSystem::Instance().provideNextFilename(m_currentFileName);
		ret = m_pimpl->parse();
	}
}

Parser::~Parser() = default;