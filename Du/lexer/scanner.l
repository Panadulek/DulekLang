%{
#include "parser.hpp" 
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>


typedef yy::parser::token token;


#define YY_DECL int yylex(yy::parser::semantic_type *yylval)

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
%}


%option noyywrap

%%

"return" { return token::RET_STMT; }
"true"   { return token::TRUE_TOKEN; }
"false"  { return token::FALSE_TOKEN; }

"==" { return token::EQ; }
"!=" { return token::NE; }
"<=" { return token::LE; }
">=" { return token::GE; }
"<"  { return token::LT; }
">"  { return token::GT; }

"->" { return token::ARROW_TOKEN; }

"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"(" { return '('; }
")" { return ')'; }
";" { return ';'; }
"," { return ','; }
"{" { return '{'; }
"}" { return '}'; }
"[" { return '['; }
"]" { return ']'; }
"=" { return '='; }

\"([^\"\\]|\\.)*\" {
    char* str = yytext;
    const size_t len = strlen(str);
    str[len - 1] = '\0';
    memmove(str, str + 1, len - 1);
    
    yylval->strval = strdup(str);
    return token::CONST_STR;
}

[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval->strval = strdup(yytext); /* Strzałka */
    return token::ID_TOKEN;
}

[0-9]+  { 
    /* Strzałka + rzutowanie */
    yylval->intval = static_cast<int>(std::stoull(yytext));  
    return token::NUMBER_TOKEN; 
}

[ \t\n]+ { }

.       { }

%%

