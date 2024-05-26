%{
#include "parser.hpp"
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
%}

%%
[0-9]+  { yylval.intval = atoi(yytext); printf("number: %d\n", yylval.intval); return NUMBER; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"(" { return '('; }
")" { return ')'; }
[ \t\n]+ { /* Ignoruj bia³e znaki */ }
.       {  }

%%

int yywrap() {
    return 1;
}