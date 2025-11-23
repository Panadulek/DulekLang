
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* First part of user declarations.  */

/* Line 311 of lalr1.cc  */
#line 1 "parser/parser.y"

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


/* Line 311 of lalr1.cc  */
#line 70 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"


#include "parser.hpp"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc  */
#line 79 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
/* Unqualified %code blocks.  */

/* Line 318 of lalr1.cc  */
#line 42 "parser/parser.y"


    int yylex(yy::parser::semantic_type *yylval);
    #include <iostream>
    #include <format>



/* Line 318 of lalr1.cc  */
#line 93 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 380 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace yy {

/* Line 380 of lalr1.cc  */
#line 162 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
     :yydebug_ (false),
      yycdebug_ (&std::cerr)
#endif
  {
  }

  parser::~parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 3:

/* Line 678 of lalr1.cc  */
#line 87 "parser/parser.y"
    {}
    break;

  case 4:

/* Line 678 of lalr1.cc  */
#line 92 "parser/parser.y"
    {
        AstScope* scope = getActualScope();
        /* Uwaga: getElement może być potrzebny do sprawdzenia, czy zmienna istnieje */
        AstElement* lhs = scope->getElement((yysemantic_stack_[(4) - (1)].strval)); 
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable((yysemantic_stack_[(4) - (1)].strval), (yysemantic_stack_[(4) - (3)].astval), getActualScope());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
        free((yysemantic_stack_[(4) - (1)].strval)); /* Pamiętaj o zwalnianiu ID */
    }
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 101 "parser/parser.y"
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable((yysemantic_stack_[(4) - (1)].astval), (yysemantic_stack_[(4) - (3)].astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    break;

  case 6:

/* Line 678 of lalr1.cc  */
#line 106 "parser/parser.y"
    {
        AstPtr ptr((yysemantic_stack_[(2) - (1)].astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(ptr));
        /* Tutaj tworzysz Statement z deklaracji? Upewnij się, że nie dublujesz elementów w Builderze */
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt((yysemantic_stack_[(2) - (1)].astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 114 "parser/parser.y"
    {
        AstPtr ptr((yysemantic_stack_[(4) - (1)].astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(ptr));
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt((yysemantic_stack_[(4) - (1)].astval), ast_element_cast<AstExpr>((yysemantic_stack_[(4) - (3)].astval)));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 121 "parser/parser.y"
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createStmt((yysemantic_stack_[(2) - (1)].astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 126 "parser/parser.y"
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createRetStmt((yysemantic_stack_[(3) - (2)].astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 141 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createAddExpr((yysemantic_stack_[(3) - (1)].astval), (yysemantic_stack_[(3) - (3)].astval));
    }
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 145 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createSubExpr((yysemantic_stack_[(3) - (1)].astval), (yysemantic_stack_[(3) - (3)].astval));
    }
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 149 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createMulExpr((yysemantic_stack_[(3) - (1)].astval), (yysemantic_stack_[(3) - (3)].astval));
    }
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 153 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createDivExpr((yysemantic_stack_[(3) - (1)].astval), (yysemantic_stack_[(3) - (3)].astval));
    }
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 157 "parser/parser.y"
    {

        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createCmpExpr((yysemantic_stack_[(3) - (1)].astval), (yysemantic_stack_[(3) - (2)].cmp_op), (yysemantic_stack_[(3) - (3)].astval));
    }
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 162 "parser/parser.y"
    {

       
    }
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 167 "parser/parser.y"
    {
        (yyval.astval) = (yysemantic_stack_[(3) - (2)].astval);
    }
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 171 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createUnsignedConst((yysemantic_stack_[(1) - (1)].intval));
    }
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 175 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(true);
    }
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 179 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(false);
    }
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 183 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createStrConst((yysemantic_stack_[(1) - (1)].strval));
        free((yysemantic_stack_[(1) - (1)].strval));
    }
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 188 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createRef((yysemantic_stack_[(1) - (1)].strval));
        free((yysemantic_stack_[(1) - (1)].strval));
    }
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 193 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun((yysemantic_stack_[(4) - (1)].strval), getActualScope(), (yysemantic_stack_[(4) - (3)].astlist));
        free((yysemantic_stack_[(4) - (1)].strval));
    }
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 198 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun((yysemantic_stack_[(3) - (1)].strval), getActualScope(), nullptr);
        free((yysemantic_stack_[(3) - (1)].strval));
    }
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 203 "parser/parser.y"
    {
        (yyval.astval) = (yysemantic_stack_[(1) - (1)].astval);
    }
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 209 "parser/parser.y"
    { (yyval.cmp_op) = AstExpr::CMP_OPERATION::EQUAL; }
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 210 "parser/parser.y"
    { (yyval.cmp_op) = AstExpr::CMP_OPERATION::NOT_EQUAL; }
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 211 "parser/parser.y"
    { (yyval.cmp_op) = AstExpr::CMP_OPERATION::LESS_THAN; }
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 212 "parser/parser.y"
    { (yyval.cmp_op) = AstExpr::CMP_OPERATION::GREATER_THAN; }
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 213 "parser/parser.y"
    { (yyval.cmp_op) = AstExpr::CMP_OPERATION::LESS_OR_EQ; }
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 214 "parser/parser.y"
    { (yyval.cmp_op) = AstExpr::CMP_OPERATION::GREATER_OR_EQ; }
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 219 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createArrayIndexingOp((yysemantic_stack_[(2) - (1)].strval), *(yysemantic_stack_[(2) - (2)].arrayDecorator));
        free((yysemantic_stack_[(2) - (1)].strval));
        /* dimension_list (arrayDecorator) powinien być zarządzany przez createArrayIndexingOp, albo usunięty ręcznie */
        delete (yysemantic_stack_[(2) - (2)].arrayDecorator); 
    }
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 228 "parser/parser.y"
    {
        (yyval.astlist) = new AstArgs();
        (yyval.astlist)->push((yysemantic_stack_[(1) - (1)].astval));
    }
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 233 "parser/parser.y"
    {
        (yyval.astlist) = (yysemantic_stack_[(3) - (1)].astlist); /* $1 to wskaźnik na listę, po prostu dodajemy do niej */
        (yyval.astlist)->push((yysemantic_stack_[(3) - (3)].astval));
    }
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 241 "parser/parser.y"
    {
        (yyval.arrayDecorator) = new ArrayDecorator::Array();
        (yyval.arrayDecorator)->emplace_back(std::make_unique<ArrayDecorator::Dimension>(ast_element_cast<AstExpr>((yysemantic_stack_[(3) - (2)].astval))));
    }
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 246 "parser/parser.y"
    {
        (yyval.arrayDecorator) = (yysemantic_stack_[(4) - (1)].arrayDecorator);
        (yyval.arrayDecorator)->emplace_back(std::make_unique<ArrayDecorator::Dimension>(ast_element_cast<AstExpr>((yysemantic_stack_[(4) - (3)].astval))));
    }
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 254 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().varFactor().createVariable((yysemantic_stack_[(2) - (1)].strval), (yysemantic_stack_[(2) - (2)].strval), getActualScope()).release();
        free((yysemantic_stack_[(2) - (1)].strval)); // Typ zmiennej
        free((yysemantic_stack_[(2) - (2)].strval)); // Nazwa zmiennej
    }
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 260 "parser/parser.y"
    {
        (yyval.astval) = AstBuildSystem::Instance().getFactory().varFactor().createArray((yysemantic_stack_[(3) - (1)].strval), (yysemantic_stack_[(3) - (2)].strval), (yysemantic_stack_[(3) - (3)].arrayDecorator), getActualScope()).release();
        free((yysemantic_stack_[(3) - (1)].strval));
        free((yysemantic_stack_[(3) - (2)].strval));
        // dimension_list ($3) został zużyty przez createArray (zakładam, że przejmuje on ownership)
    }
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 270 "parser/parser.y"
    {
        (yyval.scopeInputList) = new ScopeDecorator::Function::CONTAINER();
        (yyval.scopeInputList)->emplace_back((yysemantic_stack_[(1) - (1)].astval));
    }
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 275 "parser/parser.y"
    {
        (yyval.scopeInputList) = (yysemantic_stack_[(3) - (1)].scopeInputList);
        (yyval.scopeInputList)->emplace_back((yysemantic_stack_[(3) - (3)].astval));
    }
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 283 "parser/parser.y"
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction((yysemantic_stack_[(5) - (1)].strval), (yysemantic_stack_[(5) - (5)].strval), nullptr);
        auto funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        (yyval.astval) = funRawPtr; 
        free((yysemantic_stack_[(5) - (1)].strval)); // Nazwa funkcji
        free((yysemantic_stack_[(5) - (5)].strval)); // Typ zwracany (to też jest ID_TOKEN<strval>!)
    }
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 293 "parser/parser.y"
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction((yysemantic_stack_[(6) - (1)].strval), (yysemantic_stack_[(6) - (6)].strval), (yysemantic_stack_[(6) - (3)].scopeInputList));
        auto funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        (yyval.astval) = funRawPtr;
        free((yysemantic_stack_[(6) - (1)].strval)); // Nazwa funkcji
        free((yysemantic_stack_[(6) - (6)].strval)); // Typ zwracany
        delete (yysemantic_stack_[(6) - (3)].scopeInputList); // scopeInputList (pointer container) - jeśli createFunction robi kopię, to trzeba usunąć. Jeśli przejmuje ownership - nie usuwaj.
    }
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 306 "parser/parser.y"
    {
         AstBuildSystem::Instance().getBuilder().exitScope();
         (yyval.astval) = (yysemantic_stack_[(4) - (1)].astval);
    }
    break;



/* Line 678 of lalr1.cc  */
#line 844 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (int yystate)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char parser::yypact_ninf_ = -41;
  const short int
  parser::yypact_[] =
  {
       -41,    39,   -41,    98,    44,   -41,   -41,   -41,   -41,    98,
      98,   -41,   163,    -4,   -17,    -7,   -41,    15,   176,   -41,
      -1,    98,    52,    98,     1,   -41,   148,   -41,   -41,   -41,
     -41,   -41,   -41,    98,    98,    98,    98,   -41,    98,    98,
      98,   -41,    73,    78,   -41,     1,   189,     3,    20,   228,
      -9,   -41,    29,   112,    98,   -41,    53,    53,   -41,   -41,
     238,   202,   215,   -41,     5,   -41,   -41,    45,   -41,    98,
      60,    50,   -41,   130,   -41,   -41,   -41,   -41,   -41,   228,
      63,    68,   -41,   -41,   -41
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  parser::yydefact_[] =
  {
         2,     0,     1,     0,    24,    23,    20,    21,    22,     0,
       0,     3,     0,    27,     0,     0,    10,    24,     0,    27,
      39,     0,     0,     0,    34,    18,     0,    28,    29,    30,
      31,    32,    33,     0,     0,     0,     0,     8,     0,     0,
       0,     6,     0,     0,     9,    40,     0,    24,    26,    35,
       0,    41,     0,     0,     0,    19,    13,    14,    15,    16,
      17,     0,     0,    11,     0,    26,     4,     0,    25,     0,
       0,     0,    37,     0,     5,     7,    45,    12,    43,    36,
       0,     0,    42,    38,    44
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  parser::yypgoto_[] =
  {
       -41,   -41,   -40,   -41,    -3,   -41,     0,   -41,    54,   -19,
     -41,   -41,   -41
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  parser::yydefgoto_[] =
  {
        -1,     1,    11,    64,    12,    38,    19,    50,    24,    14,
      52,    15,    16
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char parser::yytable_ninf_ = -1;
  const signed char
  parser::yytable_[] =
  {
        18,    13,    63,    51,    40,    41,    25,    26,    20,     3,
       4,     5,     6,     7,     8,    68,    69,    39,    46,    49,
      53,    42,     9,    67,    77,    23,    43,    54,    10,    23,
      56,    57,    58,    59,    76,    60,    61,    62,    43,     2,
      49,    23,    13,     3,     4,     5,     6,     7,     8,    20,
      78,    73,    82,    70,    71,    81,     9,    47,     5,     6,
       7,     8,    10,    80,    13,    21,    79,    22,    84,     9,
      23,    35,    36,    20,    45,    10,    48,     3,     4,     5,
       6,     7,     8,    17,     5,     6,     7,     8,     0,     0,
       9,     0,     0,     0,     0,     9,    10,     0,     0,     0,
       0,    10,    65,    17,     5,     6,     7,     8,     0,     0,
       0,     0,     0,     0,     0,     9,     0,     0,     0,     0,
       0,    10,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,    72,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
       0,     0,     0,     0,     0,     0,     0,    83,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,     0,     0,
       0,     0,    55,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,     0,     0,    37,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,     0,     0,    44,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,     0,
       0,    66,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     0,     0,    74,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,     0,     0,    75,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    34,    35,    36
  };

  /* YYCHECK.  */
  const signed char
  parser::yycheck_[] =
  {
         3,     1,    42,    22,    21,    22,     9,    10,     5,     4,
       5,     6,     7,     8,     9,    24,    25,    21,    21,    22,
      23,    28,    17,     3,    64,    26,    23,    26,    23,    26,
      33,    34,    35,    36,    29,    38,    39,    40,    23,     0,
      43,    26,    42,     4,     5,     6,     7,     8,     9,     5,
       5,    54,    71,    24,    25,     5,    17,     5,     6,     7,
       8,     9,    23,     3,    64,    21,    69,    23,     5,    17,
      26,    18,    19,     5,    20,    23,    24,     4,     5,     6,
       7,     8,     9,     5,     6,     7,     8,     9,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    17,    23,    -1,    -1,    -1,
      -1,    23,    24,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    23,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    24,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    22,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    22,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    22,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    22,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    22,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  parser::yystos_[] =
  {
         0,    31,     0,     4,     5,     6,     7,     8,     9,    17,
      23,    32,    34,    36,    39,    41,    42,     5,    34,    36,
       5,    21,    23,    26,    38,    34,    34,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    22,    35,    21,
      21,    22,    28,    23,    22,    38,    34,     5,    24,    34,
      37,    39,    40,    34,    26,    24,    34,    34,    34,    34,
      34,    34,    34,    32,    33,    24,    22,     3,    24,    25,
      24,    25,    27,    34,    22,    22,    29,    32,     5,    34,
       3,     5,    39,    27,     5
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,    43,    45,    42,    47,
     271,    61,    59,    40,    41,    44,    91,    93,   123,   125
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  parser::yyr1_[] =
  {
         0,    30,    31,    31,    32,    32,    32,    32,    32,    32,
      32,    33,    33,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    35,    35,
      35,    35,    35,    35,    36,    37,    37,    38,    38,    39,
      39,    40,    40,    41,    41,    42
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  parser::yyr2_[] =
  {
         0,     2,     0,     2,     4,     4,     2,     4,     2,     3,
       1,     1,     2,     3,     3,     3,     3,     3,     2,     3,
       1,     1,     1,     1,     1,     4,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     4,     2,
       3,     1,     3,     5,     6,     4
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const parser::yytname_[] =
  {
    "$end", "error", "$undefined", "ARROW_TOKEN", "RET_STMT", "ID_TOKEN",
  "CONST_STR", "NUMBER_TOKEN", "TRUE_TOKEN", "FALSE_TOKEN", "EQ", "NE",
  "LT", "GT", "LE", "GE", "'+'", "'-'", "'*'", "'/'", "UMINUS", "'='",
  "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'", "$accept",
  "program", "stmt", "stmt_list", "expr", "cmp_op", "expr_index_op",
  "expr_list", "dimension_list", "decl_expr", "decl_expr_list",
  "decl_fun_header", "decl_fun", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const parser::rhs_number_type
  parser::yyrhs_[] =
  {
        31,     0,    -1,    -1,    31,    32,    -1,     5,    21,    34,
      22,    -1,    36,    21,    34,    22,    -1,    39,    22,    -1,
      39,    21,    34,    22,    -1,    34,    22,    -1,     4,    34,
      22,    -1,    42,    -1,    32,    -1,    33,    32,    -1,    34,
      16,    34,    -1,    34,    17,    34,    -1,    34,    18,    34,
      -1,    34,    19,    34,    -1,    34,    35,    34,    -1,    17,
      34,    -1,    23,    34,    24,    -1,     7,    -1,     8,    -1,
       9,    -1,     6,    -1,     5,    -1,     5,    23,    37,    24,
      -1,     5,    23,    24,    -1,    36,    -1,    10,    -1,    11,
      -1,    12,    -1,    13,    -1,    14,    -1,    15,    -1,     5,
      38,    -1,    34,    -1,    37,    25,    34,    -1,    26,    34,
      27,    -1,    38,    26,    34,    27,    -1,     5,     5,    -1,
       5,     5,    38,    -1,    39,    -1,    40,    25,    39,    -1,
       5,    23,    24,     3,     5,    -1,     5,    23,    40,    24,
       3,     5,    -1,    41,    28,    33,    29,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  parser::yyprhs_[] =
  {
         0,     0,     3,     4,     7,    12,    17,    20,    25,    28,
      32,    34,    36,    39,    43,    47,    51,    55,    59,    62,
      66,    68,    70,    72,    74,    76,    81,    85,    87,    89,
      91,    93,    95,    97,    99,   102,   104,   108,   112,   117,
     120,   124,   126,   130,   136,   143
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  parser::yyrline_[] =
  {
         0,    86,    86,    87,    91,   100,   105,   113,   120,   125,
     130,   134,   135,   140,   144,   148,   152,   156,   161,   166,
     170,   174,   178,   182,   187,   192,   197,   202,   209,   210,
     211,   212,   213,   214,   218,   228,   232,   240,   245,   253,
     259,   269,   274,   282,   292,   305
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      23,    24,    18,    16,    25,    17,     2,    19,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    22,
       2,    21,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    26,     2,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    28,     2,    29,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    20
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int parser::yyeof_ = 0;
  const int parser::yylast_ = 257;
  const int parser::yynnts_ = 13;
  const int parser::yyempty_ = -2;
  const int parser::yyfinal_ = 2;
  const int parser::yyterror_ = 1;
  const int parser::yyerrcode_ = 256;
  const int parser::yyntokens_ = 30;

  const unsigned int parser::yyuser_token_number_max_ = 271;
  const parser::token_number_type parser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // yy

/* Line 1054 of lalr1.cc  */
#line 1366 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 312 "parser/parser.y"


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
