%{
/* This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "rust.h"

#if !defined(YYLLOC_DEFAULT)
# define YYLLOC_DEFAULT(Current, Rhs, N)                           \
    do                                                             \
        if (N)                                                     \
        {                                                          \
            (Current).line = YYRHSLOC(Rhs, 1).line;                \
            (Current).column = YYRHSLOC(Rhs, 1).column;            \
        }                                                          \
        else                                                       \
        {                                                          \
            (Current).line = YYRHSLOC(Rhs, 0).line;                \
            (Current).column = YYRHSLOC(Rhs, 0).column;            \
        }                                                          \
    while (0)
#endif

#define YYDEBUG 1

static vec<rdot, va_gc> * symStack;
extern int yylineno;

extern int yylex (void);
extern void yyerror (const char *);
%}

%union {
    rdot symbol;
    char * string;
    int integer;
}

%debug
%locations

%error-verbose
%start declarations

%token DEFUN
%token LET
%token MUT
%token WHILE
%token LOOP
%token STATIC

%token RTYPE
%token TYPE_BOOL
%token TYPE_INT
%token TYPE_UINT
%token TYPE_FLOAT

%token EQUAL_EQUAL
%token NOT_EQUAL
%token LESS
%token GREATER
%token LESS_EQUAL
%token GREATER_EQUAL

%token<string> STRING
%token<string> IDENTIFIER
%token<integer> INTEGER

%type<symbol> funcdef
%type<symbol> decl
%type<symbol> target_ident
%type<symbol> target
%type<symbol> suite
%type<symbol> statement_list
%type<symbol> statement
%type<symbol> primary
%type<symbol> type
%type<symbol> call
%type<symbol> literal
%type<symbol> expression_stmt
%type<symbol> expression
%type<symbol> arguments
%type<symbol> argument_list

%left '='
%left '-' '+'

%%

declarations: /* epsilon */
            | declarations decl
            { dot_pass_pushDecl ($2); }
            ;

target_ident: IDENTIFIER
            { $$ = rdot_build_identifier ($1); }
            ;

funcdef: DEFUN target_ident '(' ')' '{' suite '}'
       { $$ = rdot_build_fndecl ($2, NULL_DOT, NULL_DOT, $6); }
       | DEFUN target_ident '(' ')' RTYPE type '{' suite '}'
       { $$ = rdot_build_fndecl ($2, NULL_DOT, $6, $8); }
       ;

decl: funcdef
    ;

suite: statement_list
     { $$ = symStack->pop () }
     ;

statement_list: statement_list statement
              {
		  RDOT_CHAIN ($1) = $2;
		  $$ = $2;
	      }
              | statement
              {
		  vec_safe_push (symStack, $1);
		  $$ = $1;
	      }
              ;

statement: expression ';'
         { $$ = $1; }
         ;

expression: expression_stmt
          ;

expression_stmt: target '=' expression_stmt
          { $$ = rdot_build_decl2 (D_MODIFY_EXPR, $1, $3); }
          | expression_stmt '+' expression_stmt
          { $$ = rdot_build_decl2 (D_ADD_EXPR, $1, $3); }
          | '(' expression_stmt ')'
          { $$ = $2; }
          | primary
          ;

target: IDENTIFIER
      { $$ = rdot_build_identifier ($1); }
      | LET IDENTIFIER
      { $$ = rdot_build_identifier ($2); }
      ;

literal: INTEGER
       { $$ = rdot_build_integer ($1); }
       | STRING
       { $$ = rdot_build_string ($1); }
       | IDENTIFIER
       { $$ = rdot_build_identifier ($1); }
       ;

type: TYPE_BOOL
    { $$ = rdot_build_decl1 (RTYPE_BOOL, NULL_DOT); }
    | TYPE_INT
    { $$ = rdot_build_decl1 (RTYPE_INT, NULL_DOT); }
    | TYPE_FLOAT
    { $$ = rdot_build_decl1 (RTYPE_FLOAT, NULL_DOT); }
    | TYPE_UINT
    { $$ = rdot_build_decl1 (RTYPE_UINT, NULL_DOT); }
    ;

argument_list: argument_list ',' expression
             {
		 RDOT_CHAIN ($1) = $3;
		 $$ = $3;
	     }
             | expression
	     {
		 vec_safe_push (symStack, $1);
		 $$ = $1;
	     }
             ;

arguments: argument_list
         { $$ = symStack->pop (); }
         ;

call: target_ident '(' arguments ')'
    { $$ = rdot_build_decl2 (D_CALL_EXPR, $1, $3); }
    ;

primary: literal
       | call
       ;

%%

void yyerror (const char * msg)
{
    error ("%s at line %i\n", msg, yylineno);
}
