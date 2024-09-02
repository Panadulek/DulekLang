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
#include "../ast/AstList.hpp"
#include "../Terminal/Terminal.hpp"
#include "../ast/VariableDecorator.hpp"
#include<iostream>
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
    #include "../ast/VariableDecorator.hpp"
}

%union {
    int intval;
    AstElement* astval;
    AstList* astlist;
    char* strval;
    ScopeDecorator::Function::CONTAINER* scopeInputList;
    VariableDecorator::Array* arrayDecorator;
}

%token ARROW_TOKEN RET_STMT
%token<strval> ID_TOKEN CONST_STR
%token<intval> NUMBER_TOKEN
%debug
%left '+' '-'
%left '*' '/'

%type <astval> expr decl_expr decl_fun decl_fun_header stmt
%type <astlist> expr_list 
%type <scopeInputList>decl_expr_list
%type <arrayDecorator> dimension_list
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
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt($1);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    | decl_expr '=' expr ';'
    {
        AstPtr ptr($1);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(ptr));
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt($1, ast_element_cast<AstExpr>($3));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    | expr ';'
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createStmt($1);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    | RET_STMT expr ';'
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createRetStmt($2);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    | decl_fun
     
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
    | CONST_STR
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createStrConst($1);
        free($1);
    }
    | ID_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createRef($1);
        free($1);
    }
    | ID_TOKEN '(' expr_list ')'
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun($1,  getActualScope(), $3);
        free($1);
    }
    | ID_TOKEN dimension_list // array operator
    {
    
    }
    | ID_TOKEN '(' ')'
    {
        
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun($1,  getActualScope(), nullptr);
        free($1);
    }
;

expr_list:
    |
    expr {
        $$ = new AstArgs();
        $$->push($1);
    }
    | expr_list ',' expr
    {
        $$->push($3);
    }
    ;

dimension_list:
    '[' expr ']' 
    {
        $$ = new VariableDecorator::Array();
        $$->emplace_back(std::make_unique<VariableDecorator::Dimension>(ast_element_cast<AstExpr>($2)));
    }
    | dimension_list '[' expr ']'
    {
        $$ = $1;
        $$->emplace_back(std::make_unique<VariableDecorator::Dimension>(ast_element_cast<AstExpr>($3)));
    }
    ;
decl_expr:
    ID_TOKEN ID_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory(). varFactor().createVariable($1, $2, getActualScope()).release();
        free($1);
        free($2);
    }
    | ID_TOKEN ID_TOKEN dimension_list
    {
        $$ = AstBuildSystem::Instance().getFactory().varFactor().createArray($1, $2, $3, getActualScope()).release();
        free($1);
        free($2);
    }
;


decl_expr_list:
    decl_expr
    {
        $$ = new ScopeDecorator::Function::CONTAINER();
        $$->emplace_back($1);
    }
    | decl_expr_list ',' decl_expr
    {
        $$->emplace_back($3);
    }
    ;

decl_fun_header: 
    ID_TOKEN '('  ')' ARROW_TOKEN ID_TOKEN 
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction($1, $5, nullptr);
        auto funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        $$ = funRawPtr;  // U¿ywasz tutaj AstElement*
        free($1);
    } 
    |
    ID_TOKEN '(' decl_expr_list ')' ARROW_TOKEN ID_TOKEN 
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction($1, $6, $3);
        auto funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        $$ = funRawPtr;  // U¿ywasz tutaj AstElement*
        free($1);
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
