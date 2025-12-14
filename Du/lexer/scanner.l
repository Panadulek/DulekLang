%{
#include "parser.hpp" 
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Definicja funkcji yylex zgodna z api.token.constructor
#define YY_DECL yy::parser::symbol_type yylex()

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

// Globalna lub statyczna lokalizacja do śledzenia pozycji w pliku
static yy::parser::location_type loc;

%}

%option noyywrap nounput batch debug noinput

%{
  // Kod wykonywany przy każdym dopasowaniu
  #define YY_USER_ACTION loc.columns(yyleng);
%}

%%

%{
  // Przesunięcie początku lokalizacji na poprzedni koniec przy każdym wejściu do yylex
  loc.step();
%}

"return" { return yy::parser::make_RET_STMT(loc); }
"true"   { return yy::parser::make_TRUE_TOKEN(loc); }
"false"  { return yy::parser::make_FALSE_TOKEN(loc); }

"==" { return yy::parser::make_EQ(loc); }
"!=" { return yy::parser::make_NE(loc); }
"<=" { return yy::parser::make_LE(loc); }
">=" { return yy::parser::make_GE(loc); }
"<"  { return yy::parser::make_LT(loc); }
">"  { return yy::parser::make_GT(loc); }

"->" { return yy::parser::make_ARROW_TOKEN(loc); }

"+" { return yy::parser::make_PLUS(loc); }
"-" { return yy::parser::make_MINUS(loc); }
"*" { return yy::parser::make_MULT(loc); }
"/" { return yy::parser::make_DIV(loc); }
"(" { return yy::parser::make_LPAREN(loc); }
")" { return yy::parser::make_RPAREN(loc); }
";" { return yy::parser::make_SEMICOLON(loc); }
"," { return yy::parser::make_COMMA(loc); }
"{" { return yy::parser::make_LBRACE(loc); }
"}" { return yy::parser::make_RBRACE(loc); }
"[" { return yy::parser::make_LBRACKET(loc); }
"]" { return yy::parser::make_RBRACKET(loc); }
"=" { return yy::parser::make_ASSIGN(loc); }

\"([^\"\\]|\\.)*\" {
    std::string str(yytext);
    // Usunięcie cudzysłowów
    if (str.length() >= 2) {
        str = str.substr(1, str.length() - 2);
    }
    return yy::parser::make_CONST_STR(str, loc);
}

[a-zA-Z_][a-zA-Z0-9_]* { 
    return yy::parser::make_ID_TOKEN(std::string(yytext), loc);
}

[0-9]+  { 
    int val = 0;
    try {
        val = std::stoi(yytext);
    } catch (...) {
        val = 0; // Fallback or error handling
    }
    return yy::parser::make_NUMBER_TOKEN(val, loc); 
}

[ \t]+ { 
    loc.step(); 
}

\n {
    loc.lines(1); 
    loc.step();
}

.       { 
    std::cerr << "Unexpected character: " << yytext << std::endl;
    // Opcjonalnie rzuć błąd lub zwróć token błędu
}

<<EOF>> { return yy::parser::make_YYEOF(loc); }

%%

