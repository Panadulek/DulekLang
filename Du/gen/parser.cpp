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





#include "parser.hpp"


// Unqualified %code blocks.
#line 31 "parser/parser.y"

    #include <iostream>
    #include <format>
    yy::parser::symbol_type yylex();

    AstScope* getActualScope()
    {
        return AstBuildSystem::Instance().getBuilder().getActualScope();
    }

#line 57 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"


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
#line 149 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"

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
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_dimension_list: // dimension_list
        value.YY_MOVE_OR_COPY< ArrayDecorator::Array* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expr_list: // expr_list
        value.YY_MOVE_OR_COPY< AstArgs* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_decl_expr: // decl_expr
      case symbol_kind::S_decl_fun_header: // decl_fun_header
      case symbol_kind::S_decl_fun: // decl_fun
        value.YY_MOVE_OR_COPY< AstElement* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_index_op: // expr_index_op
        value.YY_MOVE_OR_COPY< AstExpr* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_cmp_op: // cmp_op
        value.YY_MOVE_OR_COPY< AstExpr::CMP_OPERATION > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_decl_expr_list: // decl_expr_list
        value.YY_MOVE_OR_COPY< ScopeDecorator::Function::CONTAINER* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER_TOKEN: // NUMBER_TOKEN
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID_TOKEN: // ID_TOKEN
      case symbol_kind::S_CONST_STR: // CONST_STR
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_dimension_list: // dimension_list
        value.move< ArrayDecorator::Array* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expr_list: // expr_list
        value.move< AstArgs* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_decl_expr: // decl_expr
      case symbol_kind::S_decl_fun_header: // decl_fun_header
      case symbol_kind::S_decl_fun: // decl_fun
        value.move< AstElement* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_index_op: // expr_index_op
        value.move< AstExpr* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_cmp_op: // cmp_op
        value.move< AstExpr::CMP_OPERATION > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_decl_expr_list: // decl_expr_list
        value.move< ScopeDecorator::Function::CONTAINER* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER_TOKEN: // NUMBER_TOKEN
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID_TOKEN: // ID_TOKEN
      case symbol_kind::S_CONST_STR: // CONST_STR
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_dimension_list: // dimension_list
        value.copy< ArrayDecorator::Array* > (that.value);
        break;

      case symbol_kind::S_expr_list: // expr_list
        value.copy< AstArgs* > (that.value);
        break;

      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_decl_expr: // decl_expr
      case symbol_kind::S_decl_fun_header: // decl_fun_header
      case symbol_kind::S_decl_fun: // decl_fun
        value.copy< AstElement* > (that.value);
        break;

      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_index_op: // expr_index_op
        value.copy< AstExpr* > (that.value);
        break;

      case symbol_kind::S_cmp_op: // cmp_op
        value.copy< AstExpr::CMP_OPERATION > (that.value);
        break;

      case symbol_kind::S_decl_expr_list: // decl_expr_list
        value.copy< ScopeDecorator::Function::CONTAINER* > (that.value);
        break;

      case symbol_kind::S_NUMBER_TOKEN: // NUMBER_TOKEN
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_ID_TOKEN: // ID_TOKEN
      case symbol_kind::S_CONST_STR: // CONST_STR
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_dimension_list: // dimension_list
        value.move< ArrayDecorator::Array* > (that.value);
        break;

      case symbol_kind::S_expr_list: // expr_list
        value.move< AstArgs* > (that.value);
        break;

      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_decl_expr: // decl_expr
      case symbol_kind::S_decl_fun_header: // decl_fun_header
      case symbol_kind::S_decl_fun: // decl_fun
        value.move< AstElement* > (that.value);
        break;

      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_index_op: // expr_index_op
        value.move< AstExpr* > (that.value);
        break;

      case symbol_kind::S_cmp_op: // cmp_op
        value.move< AstExpr::CMP_OPERATION > (that.value);
        break;

      case symbol_kind::S_decl_expr_list: // decl_expr_list
        value.move< ScopeDecorator::Function::CONTAINER* > (that.value);
        break;

      case symbol_kind::S_NUMBER_TOKEN: // NUMBER_TOKEN
        value.move< int > (that.value);
        break;

      case symbol_kind::S_ID_TOKEN: // ID_TOKEN
      case symbol_kind::S_CONST_STR: // CONST_STR
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

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
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
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
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_dimension_list: // dimension_list
        yylhs.value.emplace< ArrayDecorator::Array* > ();
        break;

      case symbol_kind::S_expr_list: // expr_list
        yylhs.value.emplace< AstArgs* > ();
        break;

      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_decl_expr: // decl_expr
      case symbol_kind::S_decl_fun_header: // decl_fun_header
      case symbol_kind::S_decl_fun: // decl_fun
        yylhs.value.emplace< AstElement* > ();
        break;

      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_index_op: // expr_index_op
        yylhs.value.emplace< AstExpr* > ();
        break;

      case symbol_kind::S_cmp_op: // cmp_op
        yylhs.value.emplace< AstExpr::CMP_OPERATION > ();
        break;

      case symbol_kind::S_decl_expr_list: // decl_expr_list
        yylhs.value.emplace< ScopeDecorator::Function::CONTAINER* > ();
        break;

      case symbol_kind::S_NUMBER_TOKEN: // NUMBER_TOKEN
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_ID_TOKEN: // ID_TOKEN
      case symbol_kind::S_CONST_STR: // CONST_STR
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


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
#line 84 "parser/parser.y"
                   {
    }
#line 728 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 4: // stmt: ID_TOKEN "=" expr ";"
#line 90 "parser/parser.y"
    {
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable(yystack_[3].value.as < std::string > (), yystack_[1].value.as < AstExpr* > (), getActualScope());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        yylhs.value.as < AstElement* > () = nullptr; 
    }
#line 738 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 5: // stmt: expr_index_op "=" expr ";"
#line 96 "parser/parser.y"
    {
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createAssigmentVariable(yystack_[3].value.as < AstExpr* > (), yystack_[1].value.as < AstExpr* > ());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        yylhs.value.as < AstElement* > () = nullptr;
    }
#line 748 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 6: // stmt: decl_expr ";"
#line 102 "parser/parser.y"
    {
        AstElement* decl = yystack_[1].value.as < AstElement* > ();
        AstBuildSystem::Instance().getBuilder().addElement(std::unique_ptr<AstElement>(decl));
        
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt(decl);
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        yylhs.value.as < AstElement* > () = nullptr;
    }
#line 761 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 7: // stmt: decl_expr "=" expr ";"
#line 111 "parser/parser.y"
    {
        AstElement* decl = yystack_[3].value.as < AstElement* > ();
        AstBuildSystem::Instance().getBuilder().addElement(std::unique_ptr<AstElement>(decl));

        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createDeclStmt(decl, yystack_[1].value.as < AstExpr* > ());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        yylhs.value.as < AstElement* > () = nullptr;
    }
#line 774 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 8: // stmt: expr ";"
#line 120 "parser/parser.y"
    {
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createStmt(yystack_[1].value.as < AstExpr* > ());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        yylhs.value.as < AstElement* > () = nullptr;
    }
#line 784 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 9: // stmt: RET_STMT expr ";"
#line 126 "parser/parser.y"
    {
        auto stmt_uptr = AstBuildSystem::Instance().getFactory().stmtFactor().createRetStmt(yystack_[1].value.as < AstExpr* > ());
        AstBuildSystem::Instance().getBuilder().addElement(std::move(stmt_uptr));
        yylhs.value.as < AstElement* > () = nullptr;
    }
#line 794 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 10: // stmt: decl_fun
#line 132 "parser/parser.y"
    {
        yylhs.value.as < AstElement* > () = nullptr;
    }
#line 802 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 13: // expr: expr "+" expr
#line 144 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createAddExpr(yystack_[2].value.as < AstExpr* > (), yystack_[0].value.as < AstExpr* > ());
    }
#line 810 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 14: // expr: expr "-" expr
#line 148 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createSubExpr(yystack_[2].value.as < AstExpr* > (), yystack_[0].value.as < AstExpr* > ());
    }
#line 818 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 15: // expr: expr "*" expr
#line 152 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createMulExpr(yystack_[2].value.as < AstExpr* > (), yystack_[0].value.as < AstExpr* > ());
    }
#line 826 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 16: // expr: expr "/" expr
#line 156 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createDivExpr(yystack_[2].value.as < AstExpr* > (), yystack_[0].value.as < AstExpr* > ());
    }
#line 834 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 17: // expr: expr cmp_op expr
#line 160 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createCmpExpr(yystack_[2].value.as < AstExpr* > (), yystack_[1].value.as < AstExpr::CMP_OPERATION > (), yystack_[0].value.as < AstExpr* > ());
    }
#line 842 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 18: // expr: "-" expr
#line 164 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = nullptr; 
    }
#line 850 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 19: // expr: "(" expr ")"
#line 168 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = yystack_[1].value.as < AstExpr* > ();
    }
#line 858 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 20: // expr: NUMBER_TOKEN
#line 172 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createUnsignedConst(yystack_[0].value.as < int > ());
    }
#line 866 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 21: // expr: TRUE_TOKEN
#line 176 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(true);
    }
#line 874 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 22: // expr: FALSE_TOKEN
#line 180 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createBoolConst(false);
    }
#line 882 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 23: // expr: CONST_STR
#line 184 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createStrConst(yystack_[0].value.as < std::string > ());
    }
#line 890 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 24: // expr: ID_TOKEN
#line 188 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createRef(yystack_[0].value.as < std::string > ());
    }
#line 898 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 25: // expr: ID_TOKEN "(" expr_list ")"
#line 192 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun(yystack_[3].value.as < std::string > (), getActualScope(), yystack_[1].value.as < AstArgs* > ());
    }
#line 906 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 26: // expr: ID_TOKEN "(" ")"
#line 196 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createCallFun(yystack_[2].value.as < std::string > (), getActualScope(), nullptr);
    }
#line 914 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 27: // expr: expr_index_op
#line 200 "parser/parser.y"
    {
        yylhs.value.as < AstExpr* > () = yystack_[0].value.as < AstExpr* > ();
    }
#line 922 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 28: // cmp_op: EQ
#line 206 "parser/parser.y"
         { yylhs.value.as < AstExpr::CMP_OPERATION > () = AstExpr::CMP_OPERATION::EQUAL; }
#line 928 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 29: // cmp_op: NE
#line 207 "parser/parser.y"
         { yylhs.value.as < AstExpr::CMP_OPERATION > () = AstExpr::CMP_OPERATION::NOT_EQUAL; }
#line 934 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 30: // cmp_op: LT
#line 208 "parser/parser.y"
         { yylhs.value.as < AstExpr::CMP_OPERATION > () = AstExpr::CMP_OPERATION::LESS_THAN; }
#line 940 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 31: // cmp_op: GT
#line 209 "parser/parser.y"
         { yylhs.value.as < AstExpr::CMP_OPERATION > () = AstExpr::CMP_OPERATION::GREATER_THAN; }
#line 946 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 32: // cmp_op: LE
#line 210 "parser/parser.y"
         { yylhs.value.as < AstExpr::CMP_OPERATION > () = AstExpr::CMP_OPERATION::LESS_OR_EQ; }
#line 952 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 33: // cmp_op: GE
#line 211 "parser/parser.y"
         { yylhs.value.as < AstExpr::CMP_OPERATION > () = AstExpr::CMP_OPERATION::GREATER_OR_EQ; }
#line 958 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 34: // expr_index_op: ID_TOKEN dimension_list
#line 216 "parser/parser.y"
    {
        std::unique_ptr<ArrayDecorator::Array> dims(yystack_[0].value.as < ArrayDecorator::Array* > ()); // RAII
        yylhs.value.as < AstExpr* > () = AstBuildSystem::Instance().getFactory().exprFactor().createArrayIndexingOp(yystack_[1].value.as < std::string > (), *dims);
    }
#line 967 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 35: // expr_list: expr
#line 223 "parser/parser.y"
           {
        yylhs.value.as < AstArgs* > () = new AstArgs();
        yylhs.value.as < AstArgs* > ()->push(yystack_[0].value.as < AstExpr* > ());
    }
#line 976 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 36: // expr_list: expr_list "," expr
#line 228 "parser/parser.y"
    {
        yylhs.value.as < AstArgs* > () = yystack_[2].value.as < AstArgs* > ();
        yylhs.value.as < AstArgs* > ()->push(yystack_[0].value.as < AstExpr* > ());
    }
#line 985 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 37: // dimension_list: "[" expr "]"
#line 236 "parser/parser.y"
    {
        yylhs.value.as < ArrayDecorator::Array* > () = new ArrayDecorator::Array();
        yylhs.value.as < ArrayDecorator::Array* > ()->emplace_back(std::make_unique<ArrayDecorator::Dimension>(yystack_[1].value.as < AstExpr* > ()));
    }
#line 994 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 38: // dimension_list: dimension_list "[" expr "]"
#line 241 "parser/parser.y"
    {
        yylhs.value.as < ArrayDecorator::Array* > () = yystack_[3].value.as < ArrayDecorator::Array* > ();
        yylhs.value.as < ArrayDecorator::Array* > ()->emplace_back(std::make_unique<ArrayDecorator::Dimension>(yystack_[1].value.as < AstExpr* > ()));
    }
#line 1003 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 39: // decl_expr: ID_TOKEN ID_TOKEN
#line 249 "parser/parser.y"
    {
        yylhs.value.as < AstElement* > () = AstBuildSystem::Instance().getFactory().varFactor().createVariable(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > (), getActualScope()).release();
    }
#line 1011 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 40: // decl_expr: ID_TOKEN ID_TOKEN dimension_list
#line 253 "parser/parser.y"
    {
        yylhs.value.as < AstElement* > () = AstBuildSystem::Instance().getFactory().varFactor().createArray(yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > (), yystack_[0].value.as < ArrayDecorator::Array* > (), getActualScope()).release();
    }
#line 1019 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 41: // decl_expr_list: decl_expr
#line 260 "parser/parser.y"
    {
        yylhs.value.as < ScopeDecorator::Function::CONTAINER* > () = new ScopeDecorator::Function::CONTAINER();
        yylhs.value.as < ScopeDecorator::Function::CONTAINER* > ()->emplace_back(yystack_[0].value.as < AstElement* > ());
    }
#line 1028 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 42: // decl_expr_list: decl_expr_list "," decl_expr
#line 265 "parser/parser.y"
    {
        yylhs.value.as < ScopeDecorator::Function::CONTAINER* > () = yystack_[2].value.as < ScopeDecorator::Function::CONTAINER* > ();
        yylhs.value.as < ScopeDecorator::Function::CONTAINER* > ()->emplace_back(yystack_[0].value.as < AstElement* > ());
    }
#line 1037 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 43: // decl_fun_header: ID_TOKEN "(" ")" ARROW_TOKEN ID_TOKEN
#line 273 "parser/parser.y"
    {
        auto fun_uptr = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction(yystack_[4].value.as < std::string > (), yystack_[0].value.as < std::string > (), nullptr);
        AstElement* funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun_uptr));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        yylhs.value.as < AstElement* > () = nullptr; 
    }
#line 1048 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 44: // decl_fun_header: ID_TOKEN "(" decl_expr_list ")" ARROW_TOKEN ID_TOKEN
#line 281 "parser/parser.y"
    {
        auto fun_uptr = AstBuildSystem::Instance().getFactory().scopeFactor().createFunction(yystack_[5].value.as < std::string > (), yystack_[0].value.as < std::string > (), yystack_[3].value.as < ScopeDecorator::Function::CONTAINER* > ());
        AstElement* funRawPtr = AstBuildSystem::Instance().getBuilder().addElement(std::move(fun_uptr));
        AstBuildSystem::Instance().getBuilder().beginScope(funRawPtr);
        yylhs.value.as < AstElement* > () = nullptr;
    }
#line 1059 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;

  case 45: // decl_fun: decl_fun_header "{" stmt_list "}"
#line 291 "parser/parser.y"
    {
         AstBuildSystem::Instance().getBuilder().exitScope();
         yylhs.value.as < AstElement* > () = nullptr;
    }
#line 1068 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"
    break;


#line 1072 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"

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









  const signed char parser::yypact_ninf_ = -40;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -40,     4,   -40,   145,    33,   -40,   -40,   -40,   -40,   145,
     145,   -40,   174,   -15,     3,     9,   -40,    -1,   186,   -40,
      12,   145,    70,   145,    14,   -40,   159,   -40,   -40,   -40,
     -40,   -40,   -40,   145,   145,   145,   145,   -40,   145,   145,
     145,   -40,    91,    96,   -40,    14,   198,    47,    51,   234,
      -8,   -40,    -7,   111,   145,   -40,    10,    10,   -40,   -40,
     244,   210,   222,   -40,    40,   -40,   -40,    38,   145,   -40,
      50,    55,   -40,   130,   -40,   -40,   -40,   -40,   -40,   234,
      54,   -40,    56,   -40,   -40
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
      17,     0,     0,    11,     0,    26,     4,     0,     0,    25,
       0,     0,    37,     0,     5,     7,    45,    12,    43,    36,
       0,    42,     0,    38,    44
  };

  const signed char
  parser::yypgoto_[] =
  {
     -40,   -40,   -39,   -40,    -3,   -40,     0,   -40,    42,   -20,
     -40,   -40,   -40
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
      18,    13,    51,    63,     2,    39,    25,    26,     3,     4,
       5,     6,     7,     8,    68,    70,    69,    71,    46,    49,
      53,     9,    43,    40,    41,    77,    23,    10,    35,    36,
      56,    57,    58,    59,    42,    60,    61,    62,    20,    23,
      49,    54,    13,    78,     3,     4,     5,     6,     7,     8,
      81,    73,    20,    21,    67,    80,    22,     9,    82,    20,
      23,    84,    45,    10,    13,    79,    76,     0,     0,     0,
      43,     0,     0,     0,    23,    47,     5,     6,     7,     8,
       0,     0,     0,     0,     0,     0,     0,     9,     0,     0,
       0,     0,     0,    10,    48,     3,     4,     5,     6,     7,
       8,    17,     5,     6,     7,     8,     0,     0,     9,     0,
       0,     0,     0,     9,    10,     0,     0,     0,     0,    10,
      65,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,     0,     0,     0,     0,     0,     0,     0,     0,    72,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      17,     5,     6,     7,     8,     0,     0,     0,    83,     0,
       0,     0,     9,     0,     0,     0,     0,     0,    10,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,     0,
       0,     0,     0,    55,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,     0,    44,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,     0,    66,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
       0,    74,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     0,    75,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36
  };

  const signed char
  parser::yycheck_[] =
  {
       3,     1,    22,    42,     0,    20,     9,    10,     4,     5,
       6,     7,     8,     9,    22,    22,    24,    24,    21,    22,
      23,    17,    23,    20,    21,    64,    27,    23,    18,    19,
      33,    34,    35,    36,    25,    38,    39,    40,     5,    27,
      43,    27,    42,     5,     4,     5,     6,     7,     8,     9,
      70,    54,     5,    20,     3,     5,    23,    17,     3,     5,
      27,     5,    20,    23,    64,    68,    26,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    27,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    23,    24,     4,     5,     6,     7,     8,
       9,     5,     6,     7,     8,     9,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    17,    23,    -1,    -1,    -1,    -1,    23,
      24,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    23,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    24,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    21,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    21,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    21,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    21,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19
  };

  const signed char
  parser::yystos_[] =
  {
       0,    31,     0,     4,     5,     6,     7,     8,     9,    17,
      23,    32,    34,    36,    39,    41,    42,     5,    34,    36,
       5,    20,    23,    27,    38,    34,    34,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    21,    35,    20,
      20,    21,    25,    23,    21,    38,    34,     5,    24,    34,
      37,    39,    40,    34,    27,    24,    34,    34,    34,    34,
      34,    34,    34,    32,    33,    24,    21,     3,    22,    24,
      22,    24,    28,    34,    21,    21,    26,    32,     5,    34,
       5,    39,     3,    28,     5
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
  "FALSE_TOKEN", "EQ", "NE", "LT", "GT", "LE", "GE", "\"+\"", "\"-\"",
  "\"*\"", "\"/\"", "\"=\"", "\";\"", "\",\"", "\"(\"", "\")\"", "\"{\"",
  "\"}\"", "\"[\"", "\"]\"", "UMINUS", "$accept", "program", "stmt",
  "stmt_list", "expr", "cmp_op", "expr_index_op", "expr_list",
  "dimension_list", "decl_expr", "decl_expr_list", "decl_fun_header",
  "decl_fun", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    83,    83,    84,    89,    95,   101,   110,   119,   125,
     131,   138,   139,   143,   147,   151,   155,   159,   163,   167,
     171,   175,   179,   183,   187,   191,   195,   199,   206,   207,
     208,   209,   210,   211,   215,   223,   227,   235,   240,   248,
     252,   259,   264,   272,   280,   290
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


} // yy
#line 1468 "E:/LocalRepo/Compilers/DulekLang/Du/gen/parser.cpp"

#line 297 "parser/parser.y"


void yy::parser::error(const location_type& l, const std::string& m)
{
    std::string filename = (l.begin.filename) ? *l.begin.filename : "unknown";
    std::cerr << std::format("{}:{}:{}: {}\n",
                             filename,
                             l.begin.line,
                             l.begin.column,
                             m);
}
