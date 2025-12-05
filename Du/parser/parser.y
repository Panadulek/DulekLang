%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory>
#include <iostream>

#include "../ast/AstElement.hpp"
#include "../ast/AstBuildSystem.hpp"
#include "../ast/BasicType.hpp"
#include "../ast/AstList.hpp"
#include "../Terminal/Terminal.hpp"
#include "../ast/VariableDecorator.hpp"

extern int __cdecl yylex();
void yyerror(const char* s);
extern char* yytext;
extern "C" int yywrap();

AstScope* getActualScope()
{
   return AstBuildSystem::Instance().getBuilder().getActualScope();
}

using AstPtr = std::unique_ptr<AstElement>;
%}

%skeleton "lalr1.cc"  
%language "C++"       
%defines        
%locations

%code requires {
    #include "../ast/AstBuildSystem.hpp"
    #include "../ast/AstElement.hpp"
    #include <vector>
    #include "../ast/BasicType.hpp"
    #include <iostream>
    #include "../ast/VariableDecorator.hpp"
}

%code {

    int yylex(yy::parser::semantic_type *yylval, yy::parser::location_type *yylloc);
    #include <iostream>
    #include <format>
}

%union {
    int intval;
    AstElement* astval;
    AstList* astlist;
    char* strval;
    ScopeDecorator::Function::CONTAINER* scopeInputList;
    ArrayDecorator::Array* arrayDecorator;
    AstExpr::CMP_OPERATION cmp_op; /* Enum dla operatorów */
}

/* Tokeny */
%token ARROW_TOKEN RET_STMT
%token<strval> ID_TOKEN CONST_STR
%token<intval> NUMBER_TOKEN
%token TRUE_TOKEN FALSE_TOKEN 
%debug 
%token EQ NE LT GT LE GE

%nonassoc EQ NE LT GT LE GE

%left '+' '-'

%left '*' '/'

%right UMINUS

/* Typy dla reguł */
%type <astval> expr expr_index_op decl_expr decl_fun decl_fun_header stmt
%type <astlist> expr_list 
%type <scopeInputList> decl_expr_list
%type <arrayDecorator> dimension_list
%type <cmp_op> cmp_op /* Pomocnicza reguła zwracająca typ enuma */

%start program

%%

program:
    | program stmt {}
;

stmt:
      ID_TOKEN '=' expr ';'
    {
        AstScope* scope = getActualScope();
        /* Uwaga: getElement może być potrzebny do sprawdzenia, czy zmienna istnieje */
        AstElement* lhs = scope->getElement($1); 
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable($1, $3, getActualScope());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
        free($1); /* Pamiętaj o zwalnianiu ID */
    }
    | expr_index_op '=' expr ';'
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable($1, $3);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    | decl_expr ';'
    {
        AstPtr ptr($1);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(ptr));
        /* Tutaj tworzysz Statement z deklaracji? Upewnij się, że nie dublujesz elementów w Builderze */
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

/* --- GŁÓWNA REGUŁA WYRAŻEŃ --- */
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
    | expr cmp_op expr %prec EQ
    {

        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createCmpExpr($1, $2, $3);
    }
    | '-' expr %prec UMINUS
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
    | TRUE_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(true);
    }
    | FALSE_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(false);
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
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun($1, getActualScope(), $3);
        free($1);
    }
    | ID_TOKEN '(' ')'
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun($1, getActualScope(), nullptr);
        free($1);
    }
    | expr_index_op // Tablice
    {
        $$ = $1;
    }
;

cmp_op:
      EQ { $$ = AstExpr::CMP_OPERATION::EQUAL; }
    | NE { $$ = AstExpr::CMP_OPERATION::NOT_EQUAL; }
    | LT { $$ = AstExpr::CMP_OPERATION::LESS_THAN; }
    | GT { $$ = AstExpr::CMP_OPERATION::GREATER_THAN; }
    | LE { $$ = AstExpr::CMP_OPERATION::LESS_OR_EQ; }
    | GE { $$ = AstExpr::CMP_OPERATION::GREATER_OR_EQ; }
;

expr_index_op:
    ID_TOKEN dimension_list
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createArrayIndexingOp($1, *$2);
        free($1);
        /* dimension_list (arrayDecorator) powinien być zarządzany przez createArrayIndexingOp, albo usunięty ręcznie */
        delete $2; 
    }
;

expr_list:
      expr {
        $$ = new AstArgs();
        $$->push($1);
    }
    | expr_list ',' expr
    {
        $$ = $1; /* $1 to wskaźnik na listę, po prostu dodajemy do niej */
        $$->push($3);
    }
;

dimension_list:
    '[' expr ']' 
    {
        $$ = new ArrayDecorator::Array();
        $$->emplace_back(std::make_unique<ArrayDecorator::Dimension>(ast_element_cast<AstExpr>($2)));
    }
    | dimension_list '[' expr ']'
    {
        $$ = $1;
        $$->emplace_back(std::make_unique<ArrayDecorator::Dimension>(ast_element_cast<AstExpr>($3)));
    }
;

decl_expr:
    ID_TOKEN ID_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().varFactor().createVariable($1, $2, getActualScope()).release();
        free($1); // Typ zmiennej
        free($2); // Nazwa zmiennej
    }
    | ID_TOKEN ID_TOKEN dimension_list
    {
        $$ = AstBuildSystem::Instance().getFactory().varFactor().createArray($1, $2, $3, getActualScope()).release();
        free($1);
        free($2);
        // dimension_list ($3) został zużyty przez createArray (zakładam, że przejmuje on ownership)
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
        $$ = $1;
        $$->emplace_back($3);
    }
;

decl_fun_header: 
    ID_TOKEN '(' ')' ARROW_TOKEN ID_TOKEN 
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction($1, $5, nullptr);
        auto funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        $$ = funRawPtr; 
        free($1); // Nazwa funkcji
        free($5); // Typ zwracany (to też jest ID_TOKEN<strval>!)
    } 
    |
    ID_TOKEN '(' decl_expr_list ')' ARROW_TOKEN ID_TOKEN 
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction($1, $6, $3);
        auto funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        $$ = funRawPtr;
        free($1); // Nazwa funkcji
        free($6); // Typ zwracany
        delete $3; // scopeInputList (pointer container) - jeśli createFunction robi kopię, to trzeba usunąć. Jeśli przejmuje ownership - nie usuwaj.
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

void yy::parser::error(const location_type& l, const std::string& m)
{
    // Standard format: file:line:col: error message
    // If you don't use filenames yet, you can remove the filename part
    std::string filename = (l.begin.filename) ? *l.begin.filename : "unknown";

    std::cerr << std::format("{}:{}:{}: {}\n", 
                             filename, 
                             l.begin.line, 
                             l.begin.column, 
                             m);
}