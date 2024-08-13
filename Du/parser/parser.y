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
#include<iostream>
static std::vector<std::unique_ptr<AstElement>> m_AstElementList;
AstScope* getActualScope()
{
   return AstBuildSystem::Instance().getBuilder().getActualScope();
}

using AstPtr = std::unique_ptr<AstElement>;
%}


%code requires {
    #include "../ast/AstBuildSystem.hpp"
    #include "../ast/AstElement.hpp"
    #include <vector>
    #include "../ast/BasicType.hpp"
    #include <iostream>

}

%union {
    int intval;
    AstElement* astval;
    char* strval;
}

%token ARROW_TOKEN
%token<strval> ID_TOKEN
%token<intval> NUMBER_TOKEN
%debug
%left '+' '-'
%left '*' '/'

%type <astval> expr decl_expr decl_fun decl_fun_header stmt


%start program

%%

program:
    |
    program stmt{}
;


stmt:
    ID_TOKEN '=' expr ';'
    {
        AstScope* scope = getActualScope();
        AstElement* lhs = scope->getElement($1);
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable($1, $3, getActualScope());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    | decl_expr ';'
    {
        AstPtr ptr($1);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(ptr));
    }
    | expr ';'
    {
       int a = 5;
       a += 3;
    }
    | decl_fun
    {
        AstPtr ptr($1);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(ptr));
    }
;

stmt_list:
    stmt
    | stmt_list stmt
;


expr:
    expr '+' expr 
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createAddExpr($1, $3);
    }
    | expr '-' expr 
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createSubExpr($1, $3);
    }
    | expr '*' expr 
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createMulExpr($1, $3);
    }
    | expr '/' expr
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createDivExpr($1, $3);
    }
    | '-' expr %prec '*'
    {
        
    }
    | '(' expr ')' 
    {
        $$ = $2;
    }
    | NUMBER_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createUnsignedConst($1);
    }
    | ID_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().varFactor().createRef($1);
    }
;


decl_expr:
    ID_TOKEN ID_TOKEN
    {
        auto var = AstBuildSystem::Instance().getFactory(). varFactor().createVariable($1, $2, getActualScope()).release();
        $$ = var;
    }
;


decl_fun_header: 
    ID_TOKEN '('  ')' ARROW_TOKEN ID_TOKEN 
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction($1, $5, m_AstElementList).release();
        AstBuildSystem::Instance().getBuilder().beginScope(fun);
        $$ = fun;  // U¿ywasz tutaj AstElement*
    } 
;
    
decl_fun:
    decl_fun_header '{' stmt_list '}'
    {
         AstBuildSystem::Instance().getBuilder().exitScope();
         $$ = $1;
    }
;




%%



void yyerror(const char *s) {
    fprintf(stderr, "Error: %s, %s\n", s, yytext);
}
