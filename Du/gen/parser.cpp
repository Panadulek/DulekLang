// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
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

#line 69 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"


#include "parser.hpp"


// Unqualified %code blocks.
#line 43 "parser/parser.y"


    int yylex(yy::parser::semantic_type *yylval, yy::parser::location_type *yylloc);
    #include <iostream>
    #include <format>

#line 83 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 175 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}


  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_kind.
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
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
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 3: // program: program stmt
#line 88 "parser/parser.y"
                   {}
#line 647 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 4: // stmt: ID_TOKEN '=' expr ';'
#line 93 "parser/parser.y"
    {
        AstScope* scope = getActualScope();
        /* Uwaga: getElement może być potrzebny do sprawdzenia, czy zmienna istnieje */
        AstElement* lhs = scope->getElement((yystack_[3].value.strval)); 
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable((yystack_[3].value.strval), (yystack_[1].value.astval), getActualScope());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
        free((yystack_[3].value.strval)); /* Pamiętaj o zwalnianiu ID */
    }
#line 660 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 5: // stmt: expr_index_op '=' expr ';'
#line 102 "parser/parser.y"
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable((yystack_[3].value.astval), (yystack_[1].value.astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
#line 669 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 6: // stmt: decl_expr ';'
#line 107 "parser/parser.y"
    {
        AstPtr ptr((yystack_[1].value.astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(ptr));
        /* Tutaj tworzysz Statement z deklaracji? Upewnij się, że nie dublujesz elementów w Builderze */
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt((yystack_[1].value.astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
#line 681 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 7: // stmt: decl_expr '=' expr ';'
#line 115 "parser/parser.y"
    {
        AstPtr ptr((yystack_[3].value.astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(ptr));
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt((yystack_[3].value.astval), ast_element_cast<AstExpr>((yystack_[1].value.astval)));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
#line 692 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 8: // stmt: expr ';'
#line 122 "parser/parser.y"
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createStmt((yystack_[1].value.astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
#line 701 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 9: // stmt: RET_STMT expr ';'
#line 127 "parser/parser.y"
    {
        AstPtr stmt = AstBuildSystem::Instance().getFactory().stmtFactor().createRetStmt((yystack_[1].value.astval));
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt));
    }
#line 710 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 10: // stmt: decl_fun
#line 131 "parser/parser.y"
      { (yylhs.value.astval) = (yystack_[0].value.astval); }
#line 716 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 13: // expr: expr '+' expr
#line 142 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createAddExpr((yystack_[2].value.astval), (yystack_[0].value.astval));
    }
#line 724 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 14: // expr: expr '-' expr
#line 146 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createSubExpr((yystack_[2].value.astval), (yystack_[0].value.astval));
    }
#line 732 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 15: // expr: expr '*' expr
#line 150 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createMulExpr((yystack_[2].value.astval), (yystack_[0].value.astval));
    }
#line 740 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 16: // expr: expr '/' expr
#line 154 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createDivExpr((yystack_[2].value.astval), (yystack_[0].value.astval));
    }
#line 748 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 17: // expr: expr cmp_op expr
#line 158 "parser/parser.y"
    {

        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createCmpExpr((yystack_[2].value.astval), (yystack_[1].value.cmp_op), (yystack_[0].value.astval));
    }
#line 757 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 18: // expr: '-' expr
#line 163 "parser/parser.y"
    {

       
    }
#line 766 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 19: // expr: '(' expr ')'
#line 168 "parser/parser.y"
    {
        (yylhs.value.astval) = (yystack_[1].value.astval);
    }
#line 774 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 20: // expr: NUMBER_TOKEN
#line 172 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createUnsignedConst((yystack_[0].value.intval));
    }
#line 782 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 21: // expr: TRUE_TOKEN
#line 176 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(true);
    }
#line 790 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 22: // expr: FALSE_TOKEN
#line 180 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(false);
    }
#line 798 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 23: // expr: CONST_STR
#line 184 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createStrConst((yystack_[0].value.strval));
        free((yystack_[0].value.strval));
    }
#line 807 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 24: // expr: ID_TOKEN
#line 189 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createRef((yystack_[0].value.strval));
        free((yystack_[0].value.strval));
    }
#line 816 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 25: // expr: ID_TOKEN '(' expr_list ')'
#line 194 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun((yystack_[3].value.strval), getActualScope(), (yystack_[1].value.astlist));
        free((yystack_[3].value.strval));
    }
#line 825 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 26: // expr: ID_TOKEN '(' ')'
#line 199 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun((yystack_[2].value.strval), getActualScope(), nullptr);
        free((yystack_[2].value.strval));
    }
#line 834 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 27: // expr: expr_index_op
#line 204 "parser/parser.y"
    {
        (yylhs.value.astval) = (yystack_[0].value.astval);
    }
#line 842 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 28: // cmp_op: EQ
#line 210 "parser/parser.y"
         { (yylhs.value.cmp_op) = AstExpr::CMP_OPERATION::EQUAL; }
#line 848 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 29: // cmp_op: NE
#line 211 "parser/parser.y"
         { (yylhs.value.cmp_op) = AstExpr::CMP_OPERATION::NOT_EQUAL; }
#line 854 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 30: // cmp_op: LT
#line 212 "parser/parser.y"
         { (yylhs.value.cmp_op) = AstExpr::CMP_OPERATION::LESS_THAN; }
#line 860 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 31: // cmp_op: GT
#line 213 "parser/parser.y"
         { (yylhs.value.cmp_op) = AstExpr::CMP_OPERATION::GREATER_THAN; }
#line 866 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 32: // cmp_op: LE
#line 214 "parser/parser.y"
         { (yylhs.value.cmp_op) = AstExpr::CMP_OPERATION::LESS_OR_EQ; }
#line 872 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 33: // cmp_op: GE
#line 215 "parser/parser.y"
         { (yylhs.value.cmp_op) = AstExpr::CMP_OPERATION::GREATER_OR_EQ; }
#line 878 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 34: // expr_index_op: ID_TOKEN dimension_list
#line 220 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().exprFactor().createArrayIndexingOp((yystack_[1].value.strval), *(yystack_[0].value.arrayDecorator));
        free((yystack_[1].value.strval));
        /* dimension_list (arrayDecorator) powinien być zarządzany przez createArrayIndexingOp, albo usunięty ręcznie */
        delete (yystack_[0].value.arrayDecorator); 
    }
#line 889 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 35: // expr_list: expr
#line 229 "parser/parser.y"
           {
        (yylhs.value.astlist) = new AstArgs();
        (yylhs.value.astlist)->push((yystack_[0].value.astval));
    }
#line 898 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 36: // expr_list: expr_list ',' expr
#line 234 "parser/parser.y"
    {
        (yylhs.value.astlist) = (yystack_[2].value.astlist); /* $1 to wskaźnik na listę, po prostu dodajemy do niej */
        (yylhs.value.astlist)->push((yystack_[0].value.astval));
    }
#line 907 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 37: // dimension_list: '[' expr ']'
#line 242 "parser/parser.y"
    {
        (yylhs.value.arrayDecorator) = new ArrayDecorator::Array();
        (yylhs.value.arrayDecorator)->emplace_back(std::make_unique<ArrayDecorator::Dimension>(ast_element_cast<AstExpr>((yystack_[1].value.astval))));
    }
#line 916 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 38: // dimension_list: dimension_list '[' expr ']'
#line 247 "parser/parser.y"
    {
        (yylhs.value.arrayDecorator) = (yystack_[3].value.arrayDecorator);
        (yylhs.value.arrayDecorator)->emplace_back(std::make_unique<ArrayDecorator::Dimension>(ast_element_cast<AstExpr>((yystack_[1].value.astval))));
    }
#line 925 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 39: // decl_expr: ID_TOKEN ID_TOKEN
#line 255 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().varFactor().createVariable((yystack_[1].value.strval), (yystack_[0].value.strval), getActualScope()).release();
        free((yystack_[1].value.strval)); // Typ zmiennej
        free((yystack_[0].value.strval)); // Nazwa zmiennej
    }
#line 935 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 40: // decl_expr: ID_TOKEN ID_TOKEN dimension_list
#line 261 "parser/parser.y"
    {
        (yylhs.value.astval) = AstBuildSystem::Instance().getFactory().varFactor().createArray((yystack_[2].value.strval), (yystack_[1].value.strval), (yystack_[0].value.arrayDecorator), getActualScope()).release();
        free((yystack_[2].value.strval));
        free((yystack_[1].value.strval));
        // dimension_list ($3) został zużyty przez createArray (zakładam, że przejmuje on ownership)
    }
#line 946 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 41: // decl_expr_list: decl_expr
#line 271 "parser/parser.y"
    {
        (yylhs.value.scopeInputList) = new ScopeDecorator::Function::CONTAINER();
        (yylhs.value.scopeInputList)->emplace_back((yystack_[0].value.astval));
    }
#line 955 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 42: // decl_expr_list: decl_expr_list ',' decl_expr
#line 276 "parser/parser.y"
    {
        (yylhs.value.scopeInputList) = (yystack_[2].value.scopeInputList);
        (yylhs.value.scopeInputList)->emplace_back((yystack_[0].value.astval));
    }
#line 964 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 43: // decl_fun_header: ID_TOKEN '(' ')' ARROW_TOKEN ID_TOKEN
#line 284 "parser/parser.y"
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction((yystack_[4].value.strval), (yystack_[0].value.strval), nullptr);
        auto funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        (yylhs.value.astval) = funRawPtr; 
        free((yystack_[4].value.strval)); // Nazwa funkcji
        free((yystack_[0].value.strval)); // Typ zwracany (to też jest ID_TOKEN<strval>!)
    }
#line 977 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 44: // decl_fun_header: ID_TOKEN '(' decl_expr_list ')' ARROW_TOKEN ID_TOKEN
#line 294 "parser/parser.y"
    {
        auto fun = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction((yystack_[5].value.strval), (yystack_[0].value.strval), (yystack_[3].value.scopeInputList));
        auto funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        (yylhs.value.astval) = funRawPtr;
        free((yystack_[5].value.strval)); // Nazwa funkcji
        free((yystack_[0].value.strval)); // Typ zwracany
        delete (yystack_[3].value.scopeInputList); // scopeInputList (pointer container) - jeśli createFunction robi kopię, to trzeba usunąć. Jeśli przejmuje ownership - nie usuwaj.
    }
#line 991 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 45: // decl_fun: decl_fun_header '{' stmt_list '}'
#line 307 "parser/parser.y"
    {
         AstBuildSystem::Instance().getBuilder().exitScope();
         (yylhs.value.astval) = (yystack_[3].value.astval);
    }
#line 1000 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;


#line 1004 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char parser::yypact_ninf_ = -41;

  const signed char parser::yytable_ninf_ = -1;

  const short
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

  const signed char
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

  const signed char
  parser::yypgoto_[] =
  {
     -41,   -41,   -40,   -41,    -3,   -41,     0,   -41,    54,   -19,
     -41,   -41,   -41
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     1,    11,    64,    12,    38,    19,    50,    24,    14,
      52,    15,    16
  };

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

  const signed char
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

  const signed char
  parser::yyr1_[] =
  {
       0,    30,    31,    31,    32,    32,    32,    32,    32,    32,
      32,    33,    33,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    35,    35,
      35,    35,    35,    35,    36,    37,    37,    38,    38,    39,
      39,    40,    40,    41,    41,    42
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     4,     4,     2,     4,     2,     3,
       1,     1,     2,     3,     3,     3,     3,     3,     2,     3,
       1,     1,     1,     1,     1,     4,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     4,     2,
       3,     1,     3,     5,     6,     4
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "ARROW_TOKEN",
  "RET_STMT", "ID_TOKEN", "CONST_STR", "NUMBER_TOKEN", "TRUE_TOKEN",
  "FALSE_TOKEN", "EQ", "NE", "LT", "GT", "LE", "GE", "'+'", "'-'", "'*'",
  "'/'", "UMINUS", "'='", "';'", "'('", "')'", "','", "'['", "']'", "'{'",
  "'}'", "$accept", "program", "stmt", "stmt_list", "expr", "cmp_op",
  "expr_index_op", "expr_list", "dimension_list", "decl_expr",
  "decl_expr_list", "decl_fun_header", "decl_fun", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    87,    87,    88,    92,   101,   106,   114,   121,   126,
     131,   135,   136,   141,   145,   149,   153,   157,   162,   167,
     171,   175,   179,   183,   188,   193,   198,   203,   210,   211,
     212,   213,   214,   215,   219,   229,   233,   241,   246,   254,
     260,   270,   275,   283,   293,   306
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
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
    // Last valid token kind.
    const int code_max = 271;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 1445 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"

#line 313 "parser/parser.y"


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
