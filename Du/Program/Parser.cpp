#include "Parser.hpp"
#include "../gen/parser.hpp" 

#include <cstdio>
#include <cstdint>
#include "../ast/AstCast.hpp"
#include "../ast/AstBuildSystem.hpp"

// Flex nadal u¿ywa globalnego yyin (chyba ¿e te¿ go zwin¹³eœ w klasê, ale za³ó¿my standard)
extern FILE* yyin;

// USUÑ TO: extern int yyparse(); 
// USUÑ TO: extern int yydebug;

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
#ifdef _WIN32
        errno_t err = fopen_s(&yyin, filename.data(), "r");
        if (err == 0 && yyin)
            return true;
#else
        yyin = fopen(filename.data(), "r");
        if (yyin)
            return true;
#endif
        return false;
    }

    ParserRetCodes parse(bool debug)
    {
        // 1. Tworzymy obiekt parsera
        yy::parser parser_obj;

        // 2. Ustawiamy debugowanie (jeœli w³¹czone w Bisonie %debug)
#if YYDEBUG
        parser_obj.set_debug_level(debug ? 1 : 0);
#endif

        // 3. Odpalamy parsowanie (zwraca 0 jak OK, 1 jak b³¹d)
        int result = parser_obj.parse();

        if (result == 0)
            return ParserRetCodes::OK;
        else
            return ParserRetCodes::SYNTAX_ERROR;
    }

    ~Impl() = default;
};

// Konstruktor
Parser::Parser(std::string_view filename, bool debug)
    : m_currentFileName(filename), m_currentLine(0), m_pimpl(std::make_unique<Impl>()), m_debugParser(debug)
{
}

bool Parser::parse()
{
    if (m_currentFileName.empty())
        return false;

    const bool isOpenFile = m_pimpl->openFile(m_currentFileName);

    if (isOpenFile)
    {
        AstScope::GlobalApi::addFile(m_currentFileName);
        AstBuildSystem::Instance().getBuilder().beginScope(AstScope::GlobalApi::getGlobalScopeForFile(m_currentFileName));

        // Przekazujemy flagê debug do metody impl
        return m_pimpl->parse(m_debugParser) == Parser::Impl::ParserRetCodes::OK;
    }
    return false;
}

Parser::~Parser() = default;