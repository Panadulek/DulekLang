%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
extern int __cdecl yylex();
void yyerror(const char* s);
extern char* yytext;
extern "C" int yywrap();
%}

%union {
    int intval;
}


%token<intval> NUMBER

%left '+' '-'
%left '*' '/'


%type <intval> expr

%%
program:
    program expr { printf("WYNIK: %d", $2)}
    |
    ;


expr:
    expr '+' expr 
    { 
        $$ = $1 + $3;
    }
    | expr '-' expr 
    {
        $$ = $1 - $3;
    }
    | expr '*' expr 
    {
        $$ = $1 * $3;
    }
    | expr '/' expr
    {
        $$ = $1 / $3;
    }
    | '-' expr %prec '-'
    {
        $$ = -$2;
    }
    | '(' expr ')' 
    {
        $$ = $2;
    }
    | NUMBER 
    {
        $$ = $1;
    }
    ;
%%


void yyerror(const char *s) {
    fprintf(stderr, "Error: %s, %s\n", s, yytext);
}
