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
    do								   \
	if (N)							   \
	{							   \
	    (Current).line = YYRHSLOC(Rhs, 1).line;		   \
	    (Current).column = YYRHSLOC(Rhs, 1).column;		   \
	}							   \
	else							   \
	{							   \
	    (Current).line = YYRHSLOC(Rhs, 0).line;		   \
	    (Current).column = YYRHSLOC(Rhs, 0).column;		   \
	}							   \
    while (0)
#endif

#define YYDEBUG 1

extern int yylineno;

extern int yylex (void);
extern void yyerror (const char *);
%}

%union {
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

%token EQUAL_EQUAL
%token NOT_EQUAL
%token LESS
%token GREATER
%token LESS_EQUAL
%token GREATER_EQUAL

%token<string> STRING
%token<string> IDENTIFIER
%token<integer> INTEGER

%left '='
%left '-' '+'
%left '*' '/'
%left EQUAL_EQUAL
%left LESS LESS_EQUAL
%left GREATER GREATER_EQUAL
%right '^'
%nonassoc UMINUS

%%

declarations: /* epsilon */
            | declarations decl
            ;

target: IDENTIFIER
      ;

var_decl: LET target '=' INTEGER ';'
        ;

expression_stmt: target '=' expression_stmt
          | expression_stmt '+' expression_stmt
          | expression_stmt '*' expression_stmt
          | expression_stmt LESS expression_stmt
          | expression_stmt GREATER expression_stmt
          | expression_stmt EQUAL_EQUAL expression_stmt
          | expression_stmt NOT_EQUAL expression_stmt
          | '(' expression_stmt ')'
          | primary
          ;

expression: expression_stmt
          ;

stmt: var_decl
    | expression
    ;

statement: stmt ';'

suite: suite statement
     | statement
     ;

funcdef: DEFUN IDENTIFIER '(' ')' '{' suite '}'
       ;

decl: funcdef
    ;

literal: INTEGER
       | IDENTIFIER
       ;

call: primary '(' ')'
    ;

primary: literal
       | call
       ;
%%

void yyerror (const char * msg)
{
    error ("%s at line %i\n", msg, yylineno);
}
