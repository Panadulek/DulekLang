%skeleton "lalr1.cc"
%language "C++"
%defines
%locations
%debug

%define api.value.type variant
%define api.token.constructor

%code requires {
    #include <cstdio>
    #include <cstdlib>
    #include <cstring>
    #include <vector>
    #include <memory>
    #include <iostream>
    #include <algorithm>

    #include "../ast/AstElement.hpp"
    #include "../ast/AstBuildSystem.hpp"
    #include "../ast/BasicType.hpp"
    #include "../ast/AstList.hpp"
    #include "../Terminal/Terminal.hpp"
    #include "../ast/VariableDecorator.hpp"
    #include "../ast/AstExpr.hpp"
    #include "../ast/ScopeDecorator.hpp"

    AstScope* getActualScope();
}

%code {
    #include <iostream>
    #include <format>
    yy::parser::symbol_type yylex();

    AstScope* getActualScope()
    {
        return AstBuildSystem::Instance().getBuilder().getActualScope();
    }
}

%token ARROW_TOKEN RET_STMT
%token <std::string> ID_TOKEN CONST_STR
%token <int> NUMBER_TOKEN
%token IF_TOKEN ELSE_TOKEN WHILE_TOKEN FOR_TOKEN
%token TRUE_TOKEN FALSE_TOKEN
%token EQ NE LT GT LE GE

%token PLUS "+"
%token MINUS "-"
%token MULT "*"
%token DIV "/"
%token ASSIGN "="
%token SEMICOLON ";"
%token COMMA ","
%token LPAREN "("
%token RPAREN ")"
%token LBRACE "{"
%token RBRACE "}"
%token LBRACKET "["
%token RBRACKET "]"

%nonassoc EQ NE LT GT LE GE
%left PLUS MINUS
%left MULT DIV
%right UMINUS

/* ZMIANA: Powrót do surowych wskaźników w wariancie, aby uniknąć problemów z kopiowaniem unique_ptr */
%type <AstExpr*> expr expr_index_op
%type <AstElement*> decl_expr decl_fun decl_fun_header stmt
%type <AstArgs*> expr_list
%type <ScopeDecorator::Function::CONTAINER*> decl_expr_list
%type <ArrayDecorator::Array*> dimension_list
%type <AstExpr::CMP_OPERATION> cmp_op

/* DESTRUKTOR: Jeśli wystąpi błąd składni, Bison wywoła to dla symboli na stosie */
// %destructor { delete $$; } <AstExpr*> <AstElement*> <AstArgs*> <ScopeDecorator::Function::CONTAINER*> <ArrayDecorator::Array*>

%start program

%%

program:
    %empty
    | program stmt {
    }
;

stmt:
      ID_TOKEN ASSIGN expr SEMICOLON
    {
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable($1, $3, getActualScope());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        $$ = nullptr; 
    }
    | expr_index_op ASSIGN expr SEMICOLON
    {
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable($1, $3);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        $$ = nullptr;
    }
    | decl_expr SEMICOLON
    {
        AstElement* decl = $1;
        AstBuildSystem::Instance().getBuilder().addElement(std::unique_ptr<AstElement>(decl));
        
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt(decl);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        $$ = nullptr;
    }
    | decl_expr ASSIGN expr SEMICOLON
    {
        AstElement* decl = $1;
        AstBuildSystem::Instance().getBuilder().addElement(std::unique_ptr<AstElement>(decl));

        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt(decl, $3);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        $$ = nullptr;
    }
    | expr SEMICOLON
    {
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createStmt($1);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        $$ = nullptr;
    }
    | RET_STMT expr SEMICOLON
    {
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createRetStmt($2);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        $$ = nullptr;
    }
    | decl_fun
    {
        $$ = nullptr;
    }
    | IF_TOKEN LPAREN expr RPAREN
    {
        AstBuildSystem::Instance().getFactory().stmtFactor().createConditionBlockStmt($3);
	} LBRACE stmt_list RBRACE
    {
        AstBuildSystem::Instance().getBuilder().exitScope();
		$$ = nullptr;
    }
;

stmt_list:
      stmt
    | stmt_list stmt
;

expr:
      expr PLUS expr
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createAddExpr($1, $3).release();
    }
    | expr MINUS expr
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createSubExpr($1, $3).release();
    }
    | expr MULT expr
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createMulExpr($1, $3).release();
    }
    | expr DIV expr
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createDivExpr($1, $3).release();
    }
    | expr cmp_op expr %prec EQ
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createCmpExpr($1, $2, $3).release();
    }
    | MINUS expr %prec UMINUS
    {
        $$ = nullptr; 
    }
    | LPAREN expr RPAREN
    {
        $$ = $2;
    }
    | NUMBER_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createUnsignedConst($1).release();
    }
    | TRUE_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(true).release();
    }
    | FALSE_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(false).release();
    }
    | CONST_STR
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createStrConst($1).release();
    }
    | ID_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createRef($1).release();
    }
    | ID_TOKEN LPAREN expr_list RPAREN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun($1, getActualScope(), $3).release();
    }
    | ID_TOKEN LPAREN RPAREN
    {
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun($1, getActualScope(), nullptr).release();
    }
    | expr_index_op
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
        std::unique_ptr<ArrayDecorator::Array> dims($2); // RAII
        $$ = AstBuildSystem::Instance().getFactory().exprFactor().createArrayIndexingOp($1, *dims).release();
    }
;

expr_list:
      expr {
        $$ = new AstArgs();
        $$->push($1);
    }
    | expr_list COMMA expr
    {
        $$ = $1;
        $$->push($3);
    }
;

dimension_list:
    LBRACKET expr RBRACKET
    {
        $$ = new ArrayDecorator::Array();
        $$->emplace_back(std::make_unique<ArrayDecorator::Dimension>($2));
    }
    | dimension_list LBRACKET expr RBRACKET
    {
        $$ = $1;
        $$->emplace_back(std::make_unique<ArrayDecorator::Dimension>($3));
    }
;

decl_expr:
    ID_TOKEN ID_TOKEN
    {
        $$ = AstBuildSystem::Instance().getFactory().varFactor().createVariable($1, $2, getActualScope()).release();
    }
    | ID_TOKEN ID_TOKEN dimension_list
    {
        $$ = AstBuildSystem::Instance().getFactory().varFactor().createArray($1, $2, $3, getActualScope()).release();
    }
;

decl_expr_list:
    decl_expr
    {
        $$ = new ScopeDecorator::Function::CONTAINER();
        $$->emplace_back($1);
    }
    | decl_expr_list COMMA decl_expr
    {
        $$ = $1;
        $$->emplace_back($3);
    }
;

decl_fun_header:
    ID_TOKEN LPAREN RPAREN ARROW_TOKEN ID_TOKEN
    {
        auto fun_uptr = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction($1, $5, nullptr);
        AstElement* funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun_uptr));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        $$ = nullptr; 
    }
    |
    ID_TOKEN LPAREN decl_expr_list RPAREN ARROW_TOKEN ID_TOKEN
    {
        auto fun_uptr = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction($1, $6, $3);
        AstElement* funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun_uptr));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        $$ = nullptr;
    }
;

decl_fun:
    decl_fun_header LBRACE stmt_list RBRACE
    {
         AstBuildSystem::Instance().getBuilder().exitScope();
         $$ = nullptr;
    }
;

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
    std::string filename = (l.begin.filename) ? *l.begin.filename : "unknown";
    std::cerr << std::format("{}:{}:{}: {}\n",
                             filename,
                             l.begin.line,
                             l.begin.column,
                             m);
}
