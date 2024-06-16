%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
extern int __cdecl yylex();
void yyerror(const char* s);
extern char* yytext;
extern "C" int yywrap();
#include "../ast/AstElement.hpp"
#include "../ast/AstBuildSystem.hpp"
#include "../ast/BasicType.hpp"

static std::vector<std::unique_ptr<AstElement>> m_AstElementList;


%}


%code requires {
    #include "../ast/AstBuildSystem.hpp"
    #include "../ast/AstElement.hpp"
    #include <vector>
    #include "../ast/BasicType.hpp"
}

%union {
    int intval;
    AstElement* astval;
    char* strval;
}

%token FUN_TOKEN ARROW_TOKEN
%token<strval> ID_TOKEN
%token<intval> NUMBER_TOKEN

%left '+' '-'
%left '*' '/'

%type <astval> exprs
%type <astval> expr
%type <astval> decl_expr
%type <astval> decl_fun
%%
program:
    program exprs {}
    |
    ;

exprs:
    expr {$$ = $1;}
    |decl_fun{$$ = $1;}
    ;

expr:
    expr '+' expr 
    { 
    }
    | expr '-' expr 
    {
    }
    | expr '*' expr 
    {
    }
    | expr '/' expr
    {
    }
    | '-' expr %prec '-'
    {
    }
    | '(' expr ')' 
    {
    }
    | NUMBER_TOKEN
    {
    }
    ;
decl_expr:
    ID_TOKEN ID_TOKEN ';'
    {
    
    }
;

decl_fun: 
    ID_TOKEN '(' args_list ')' ARROW_TOKEN ID_TOKEN
    {
        std::unique_ptr<AstElement> fun = AstBuildSystem::Instance().getFactory().createFunction($1, nullptr, BasicTypes::I8, m_AstElementList);
        AstElement* ptr = fun.get();
        $$ = ptr;
    }
    ;
args_list: 
    |expr
    { 
    }
    |args_list ',' expr 
    {
    }
    ;
%%



void yyerror(const char *s) {
    fprintf(stderr, "Error: %s, %s\n", s, yytext);
}
