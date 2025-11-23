%{
#include "parser.hpp"
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
%}

%%

"return" { return RET_STMT; }
"true"   { return TRUE_TOKEN; }
"false"  { return FALSE_TOKEN; }

"==" { return EQ; }
"!=" { return NE; }
"<=" { return LE; }
">=" { return GE; }
"<"  { return LT; }
">"  { return GT; }

"->" { return ARROW_TOKEN; }

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
    yylval.strval = strdup(str);
    return CONST_STR;
}

[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.strval = strdup(yytext);
    return ID_TOKEN;
}

[0-9]+  { 
    yylval.intval = static_cast<int>(std::stoull(yytext));  
    return NUMBER_TOKEN; 
}

[ \t\n]+ { }

.       { }

%%

int yywrap() {
    return 1;
}