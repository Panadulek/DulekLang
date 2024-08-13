%{
#include "parser.hpp"
#include <string>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
%}


%%


[a-zA-Z_][a-zA-Z0-9_]*  { 
yylval.strval = strdup(yytext);
return ID_TOKEN;
}
[0-9]+  { yylval.intval =  std::stoull(yytext);  return NUMBER_TOKEN; }

"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"(" { return '('; }
")" { return ')'; }
";" {return ';';}
"->" {return ARROW_TOKEN;}
"," {return ',';}
"{" {return '{';}
"}" {return '}';}
"=" {return '=';}
[ \t\n]+ { /* Ignoruj bia³e znaki */ }
.       {  }

%%

int yywrap() {
    return 1;
}


