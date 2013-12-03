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
  bool boolean;
  ALLOCA_ alloca_;
  qualified qual;
}

%debug
%locations

%error-verbose
%start declarations

%token IMPL "impl"
%token AS "as"
%token BREAK "break"
%token DO "do"
%token DEFUN "fn"
%token LET "let"
%token MUT "mut"
%token LOOP "loop"
%token STATIC "static"

%token RTYPE "->"
%token TYPE_BOOL "bool"
%token TYPE_INT "int"
%token TYPE_UINT "uint"
%token TYPE_FLOAT "float"
%token ACC "::"
%token ENUM 'enum'

%token EQUAL_EQUAL "=="
%token NOT_EQUAL "!=" 
%token LESS "<"
%token GREATER ">"
%token LESS_EQUAL "<="
%token GREATER_EQUAL ">="
%token STRUCT "struct"
%token WHILE "while"
%token IF "if"
%token ELSE "else"
%token SELF "self"
%token MATCH "match"
%token GOES "=>"
%token XTRUE "true"
%token XFALSE "false"

%token<string> STRING
%token<string> IDENTIFIER
%token<integer> INTEGER

%right '='
%left '+'
%left '-'
%left '*'
%left '<'
%left '>'
%left ACC
%left '.'
%left GREATER_EQUAL
%left LESS_EQUAL

%type<symbol> decl
%type<symbol> fndecl
%type<symbol> structdef
%type<symbol> enumdef
%type<symbol> enum_item
%type<symbol> enum_list
%type<symbol> impl_block
%type<symbol> literal
%type<symbol> arguments
%type<symbol> expression
%type<symbol> expression_stmt
%type<symbol> argument_list
%type<symbol> primary
%type<symbol> struct_init
%type<symbol> struct_init_list
%type<symbol> while_loop
%type<symbol> if_block
%type<symbol> else_block
%type<symbol> parameter
%type<symbol> parameters
%type<symbol> parameter_list
%type<symbol> vardecl
%type<symbol> vardecl_type
%type<symbol> literal_type
%type<symbol> target
%type<symbol> cond_stmt
%type<alloca_> modifier
%type<symbol> fntype
%type<qual> mutable
%type<boolean> reference
%type<symbol> suite
%type<symbol> param
%type<symbol> maybe_else_block
%type<symbol> impl_block_decls
%type<symbol> statement_list
%type<symbol> type
%type<symbol> statement

%%

declarations: /* epsilon */
            | declarations decl
            { dot_pass_pushDecl ($2); }
            ;

decl: fndecl
    | structdef
    | impl_block
    | enumdef
    ;

enum_item: IDENTIFIER
         { $$ = rdot_build_identifier ($1); }
         ;

enum_list: enum_list ',' enum_item
         {
	   RDOT_CHAIN ($1) = $3;
	   $$ = $3;
	 }
         | enum_item
	 {
	   vec_safe_push (symStack, $1);
	   $$ = $1;
	 }
         ;

enumdef: ENUM IDENTIFIER '{' enum_list '}'
       {
	 $$ = rdot_build_decl2 (D_STRUCT_ENUM,
				rdot_build_identifier ($2),
				symStack->pop ());
       }
       ;

impl_block: IMPL IDENTIFIER '{' impl_block_decls '}'
          {
	    $$ = rdot_build_decl2 (D_STRUCT_IMPL,
				   rdot_build_identifier ($2),
				   symStack->pop ());
	  }
          ;

impl_block_decls: impl_block_decls fndecl
                {
		  RDOT_CHAIN ($1) = $2;
		  $$ = $2;
		}
                | fndecl
                {
		  vec_safe_push (symStack, $1);
		  $$ = $1;
		}
                ;

structdef: STRUCT IDENTIFIER '{' parameters '}'
         {
	   $$ = rdot_build_decl2 (D_STRUCT_TYPE,
				  rdot_build_identifier ($2),
				  symStack->pop ())
	 }
         ;

fntype:
      { $$ = NULL_DOT; }
      | RTYPE type
      { $$ = $2; }
      ;

/* ignore for now ... */
modifier:
        { $$ = ALLOC_AUTO; }
        | '~'
	{ $$ = ALLOC_HEAP; }
        | '&'
        { $$ = ALLOC_STACK; }
        | '@'
        { $$ = ALLOC_BOX; }
        ;

param: IDENTIFIER ':' type
     {
       $$ = rdot_build_decl2 (D_PARAMETER,
			      rdot_build_identifier ($1), $3);
     }
     | SELF
     {
       $$ = rdot_build_decl2 (D_PARAMETER,
			      rdot_build_identifier ("self"),
			      rdot_build_decl1 (RTYPE_INFER, NULL_DOT));
     }
     ;

reference:
         { $$ = false; }
         | '&'
	 { $$ = true; }
         ;

parameter: reference mutable param
         {
	   RDOT_qual ($3) = $2;
	   RDOT_REFERENCE ($3) = $1;
	   $$ = $3;
	 }
         ;

parameters: parameters ',' parameter
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

parameter_list:
              { $$ = NULL_DOT; }
              | parameters
	      { $$ = symStack->pop (); }
	      ;

fndecl: DEFUN IDENTIFIER '(' parameter_list ')' fntype '{' suite '}'
      { $$ = rdot_build_fndecl (rdot_build_identifier ($2), $4, $6, $8); }
      ;

suite:
     { $$ = NULL_DOT; }
     | statement_list
     { $$ = symStack->pop (); }
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

while_loop: WHILE '(' expression ')' '{' suite '}'
          { $$ = rdot_build_decl2 (D_STRUCT_WHILE, $3, $6); }
          ;

if_block: IF '(' expression ')' '{' suite '}'
        { $$ = rdot_build_decl2 (D_STRUCT_IF, $3, $6); }
        ;

else_block: ELSE '{' suite '}'
          { $$ = rdot_build_decl1 (D_STRUCT_ELSE, $3); }
          ;

maybe_else_block:
                { $$ = NULL_DOT; }
                | else_block
		;

cond_stmt: if_block maybe_else_block
         { $$ = rdot_build_decl2 (D_STRUCT_IF, $1, $2); }
         ;

statement: expression_stmt
         | expression
         {
	   DOT_RETVAL ($1) = true;
	   $$ = $1;
	 }
         | while_loop
         | cond_stmt
         ;

mutable:
       { $$ = FINAL; }
       | MUT
       { $$ = MUTABLE; }
       ;

vardecl_type:
            { $$ = NULL_DOT; }
            | ':' type
            { $$ = $2; }
	    ;

vardecl: LET mutable IDENTIFIER vardecl_type
       {
	 rdot type = NULL_DOT;
	 if ($4)
	   type = $4;
	 else
	   type = rdot_build_decl1 (RTYPE_INFER, NULL_DOT);

	 $$ = rdot_build_varDecl (type, $2, rdot_build_identifier ($3));
       }
       ;

target: IDENTIFIER
      { $$ = rdot_build_identifier ($1); }
      | vardecl
      ;

expression_stmt: expression ';'
               { $$ = $1; }
               ;

struct_init: IDENTIFIER ':' expression
           {
	     $$ = rdot_build_decl2 (D_STRUCT_PARAM,
				    rdot_build_identifier ($1),
				    $3);
	   }
           ;

struct_init_list: struct_init_list ',' struct_init
                {
		  RDOT_CHAIN ($1) = $3;
		  $$ = $3;
		}
                | struct_init
                {
		  vec_safe_push (symStack, $1);
		  $$ = $1;
		}
                ;

expression: target '=' expression
          { $$ = rdot_build_decl2 (D_MODIFY_EXPR, $1, $3); }
          | expression '+' expression
          { $$ = rdot_build_decl2 (D_ADD_EXPR, $1, $3); }
          | expression '-' expression
          { $$ = rdot_build_decl2 (D_MINUS_EXPR, $1, $3); }
          | expression '*' expression
          { $$ = rdot_build_decl2 (D_MULT_EXPR, $1, $3); }
          | expression '<' expression
          { $$ = rdot_build_decl2 (D_LESS_EXPR, $1, $3); }
          | expression '>' expression
          { $$ = rdot_build_decl2 (D_GREATER_EXPR, $1, $3); }
          | expression GREATER_EQUAL expression
          { $$ = rdot_build_decl2 (D_GREATER_EQ_EXPR, $1, $3); }
          | expression LESS_EQUAL expression
          { $$ = rdot_build_decl2 (D_LESS_EQ_EXPR, $1, $3); }
          | expression '.' expression
          { $$ = rdot_build_decl2 (D_ATTRIB_REF, $1, $3); }
          | expression ACC expression
          { $$ = rdot_build_decl2 (D_ACC_EXPR, $1, $3); }
          | '(' expression ')'
	  { $$ = $2; }
          | primary
          ;

literal: INTEGER
       { $$ = rdot_build_integer ($1); }
       | STRING
       { $$ = rdot_build_string ($1); }
       | XTRUE
       { $$ = rdot_build_bool (true); }
       | XFALSE
       { $$ = rdot_build_bool (false); }
       | SELF
       { $$ = rdot_build_identifier ("self"); }
       | '_'
       { $$ = rdot_build_identifier ("_"); }
       ;

arguments: arguments ',' expression
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

argument_list:
             { $$ = NULL_DOT; }
             | arguments
	     { $$ = symStack->pop (); }
	     ;

primary: literal
       | IDENTIFIER '{' struct_init_list '}'
       {
	 $$ = rdot_build_decl2 (D_STRUCT_INIT, rdot_build_identifier ($1),
				symStack->pop ());
       }
       | IDENTIFIER '(' argument_list ')'
       { $$ = rdot_build_decl2 (D_CALL_EXPR, rdot_build_identifier ($1), $3); }
       | IDENTIFIER
       { $$ = rdot_build_identifier ($1); }
       ;

literal_type: TYPE_INT
            { $$ = rdot_build_decl1 (RTYPE_INT, NULL_DOT); }
            | TYPE_BOOL
            { $$ = rdot_build_decl1 (RTYPE_BOOL, NULL_DOT); }
            | IDENTIFIER
            {
	      $$ = rdot_build_decl1 (RTYPE_USER_STRUCT, 
				     rdot_build_identifier ($1));
	    }
            ;

type: modifier literal_type
    {
      RDOT_MEM_MODIFIER ($2) = $1;
      $$ = $2;
    }
    ;

%%

void yyerror (const char * msg)
{
    error ("%s at line %i\n", msg, yylineno);
}
