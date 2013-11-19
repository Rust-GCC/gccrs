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
 //%glr-parser

%error-verbose
%start declarations

%token DEFUN "fn"
%token LET "let"
%token MUT "mut"
%token WHILE "while"
%token LOOP "loop"
%token STATIC "static"

%token RTYPE "->"
%token TYPE_BOOL "bool"
%token TYPE_INT "int"
%token TYPE_UINT "uint"
%token TYPE_FLOAT "float"

%token EQUAL_EQUAL "=="
%token NOT_EQUAL "!=" 
%token LESS "<"
%token GREATER ">"
%token LESS_EQUAL "<="
%token GREATER_EQUAL ">="
%token STRUCT 'struct'

%token<string> STRING
%token<string> IDENTIFIER
%token<integer> INTEGER

%type<symbol> funcdef
%type<symbol> decl
%type<symbol> target_ident
%type<symbol> target
%type<symbol> vardecl
%type<symbol> suite
%type<symbol> statement_list
%type<symbol> statement
%type<symbol> delimstatement
%type<symbol> primary
%type<symbol> type
%type<symbol> call
%type<symbol> literal
%type<symbol> expression_stmt
%type<symbol> expression
%type<symbol> arguments
%type<symbol> argument_list
%type<symbol> parameter
%type<symbol> parameter_list
%type<symbol> parameters
%type<symbol> structdef
%type<symbol> struct_init
%type<symbol> struct_init_params
%type<symbol> struct_param

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

parameter: target_ident ':' type
         {
	   $$ = rdot_build_decl2 (D_PARAMETER, $1, $3);
	 }
         ;

parameter_list: parameter_list ',' parameter
              {
		RDOT_CHAIN ($1) = $3;
		$$ = $3;
	      }
              | parameter
	      {
		vec_safe_push (symStack, $1);
		$$ = $1;
	      }
              ;

parameters: /* lambda/empty */
          { $$ = NULL_DOT; }
          | parameter_list
	  { $$ = symStack->pop (); }
	  ;

funcdef: DEFUN target_ident '(' parameters ')' '{' suite '}'
       { $$ = rdot_build_fndecl ($2, $4, NULL_DOT, $7); }
       | DEFUN target_ident '(' parameters ')' RTYPE type '{' suite '}'
       { $$ = rdot_build_fndecl ($2, $4, $7, $9); }
       ;

structdef: STRUCT target_ident '{' parameter_list '}'
         { $$ = rdot_build_decl2 (D_STRUCT_TYPE, $2, symStack->pop ()); }
         ;

decl: funcdef
    | structdef
    ;

suite: statement_list
     { $$ = symStack->pop () }
     ;

statement_list: statement_list delimstatement
              {
		  RDOT_CHAIN ($1) = $2;
		  $$ = $2;
	      }
              | delimstatement
              {
		  vec_safe_push (symStack, $1);
		  $$ = $1;
	      }
              ;

delimstatement: statement ';'
              { $$ = $1; }
              | statement
              {
		  MAYBE_RETVAL ($1);
		  $$ = $1;
	      }
              ;

statement: expression
         | vardecl
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

vardecl: LET target_ident
       {
	 $$ = rdot_build_varDecl (rdot_build_decl1 (RTYPE_INFER, NULL_DOT),
				  FINAL, $2);
       }
       | LET target_ident ':' type
       { $$ = rdot_build_varDecl ($4, FINAL, $2); }
       | LET MUT target_ident 
       {
	 $$ = rdot_build_varDecl (rdot_build_decl1 (RTYPE_INFER, NULL_DOT),
				  MUTABLE, $3);
       }
       | LET MUT target_ident ':' type
       { $$ = rdot_build_varDecl ($5, MUTABLE, $3); }
       ;

target: target_ident
      | vardecl
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

arguments: /* lambda/empty */
         { $$ = NULL; }
         | argument_list
         { $$ = symStack->pop (); }
         ;

call: target_ident '(' arguments ')'
    { $$ = rdot_build_decl2 (D_CALL_EXPR, $1, $3); }
    ;

struct_param: target_ident ':' expression
            { $$ = rdot_build_decl2 (D_STRUCT_PARAM, $1, $3); }
            ;

struct_init_params: struct_init_params ',' struct_param
                  {
		    RDOT_CHAIN ($1) = $3;
		    $$ = $3;
		  }
                  | struct_param
                  {
		    vec_safe_push (symStack, $1);
		    $$ = $1;
		  }
                  ;

struct_init: target_ident '{' struct_init_params '}'
           { $$ = rdot_build_decl2 (D_STRUCT_INIT, $1, symStack->pop ()); }
           ;

primary: literal
       | struct_init
       | call
       ;

%%

void yyerror (const char * msg)
{
    error ("%s at line %i\n", msg, yylineno);
}
