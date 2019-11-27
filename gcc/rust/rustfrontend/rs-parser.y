%{
#include "rdot.h"
#include "rust-system.h"

static std::deque<rdot> grs_symbol_stack;
extern int yylineno;

extern int yylex (void);
extern void yyerror (const char *);
%}

%union {
  char* str;
  long int integer;
  float decimal;
  rdot node;
  bool boolean;
}

%debug
%locations
%define parse.error verbose

%token <str> IDENTIFIER
%token <str> STRING
%token <integer> INTEGER
%token <decimal> FLOAT

%token PUB
%token BREAK
%token RETURN
%token CONTINUE
%token FN
%token LET
%token MUT
%token LOOP
%token STATIC
%token STRUCT
%token IF
%token ELIF
%token ELSE
%token IMPL
%token SELF
%token POINTER
%token NS_ACC
%token EQUAL_EQUAL
%token NOT_EQUAL
%token LESS_EQUAL
%token GREATER_EQUAL
%token RTRUE
%token RFALSE

%type<node> literal
%type<node> ident
%type<node> attributeref
%type<node> atom
%type<node> call
%type<node> primary
%type<node> expression_stmt
%type<node> var_decl
%type<node> return_stmt
%type<node> parameter_list_stmt
%type<node> parameter_list
%type<node> parameter
%type<node> funcdef
%type<node> argument_list_stmt
%type<node> argument_list
%type<node> argument
%type<node> toplevel_compound_stmt
%type<node> declaration
%type<node> declaration_list
%type<node> compound_statement
%type<node> expression
%type<node> target
%type<boolean> public
%type<node> fn_return_decl
%type<node> structdef
%type<boolean> mutable
%type<node> typedecl
%type<node> struct_init
%type<node> struct_init_stmt
%type<node> struct_init_list
%type<node> struct_init_elem
%type<node> loop
%type<node> continue_stmt
%type<node> break_stmt
%type<node> if_statement
%type<node> elif_statement
%type<node> elif_block_list
%type<node> elif_block
%type<node> else_statement
%type<node> conditional
%type<node> control_structure

%left '='
%left '-' '+'
%left '*' '/'
%left '%'
%left EQUAL_EQUAL
%left LESS LESS_EQUAL
%left GREATER GREATER_EQUAL
%right '^'
%nonassoc UMINUS

%start declarations

%%

declarations: /* epsilon */
            | declarations toplevel_compound_stmt
            {
		if ($2 != NULL)
		    CompilationUnitAst::get().addNode($2);
	    }
            ;

toplevel_compound_stmt: funcdef
                      | structdef
                      ;

structdef: STRUCT ident '{' parameter_list_stmt '}'
         {
	     $$ = rdot_build_decl2 (D_STRUCT_TYPE, $2, $4);
	 }
         ;

funcdef: public FN ident '(' parameter_list_stmt ')' fn_return_decl compound_statement
       {
           $$ = rdot_build_fndecl ($3, $1, $5, $7, $8);
       }
       ;

public:
      { $$ = false; }
      | PUB
      { $$ = true; }
      ;

fn_return_decl:
              { $$ = NULL; }
              | POINTER ident
              {
		  $$ = $2;  
              }
              ;

parameter_list_stmt:
                   { $$ = NULL_DOT; }
                   | parameter_list
                   {    
		       $$ = grs_symbol_stack.front ();
		       grs_symbol_stack.pop_front ();
		   }
		   ;

parameter_list: parameter_list ',' parameter
              {
		RDOT_CHAIN($1) = $3;
		$$ = $3;
	      }
              | parameter
              {
		  grs_symbol_stack.push_front($1);
		  $$ = $1;
	      }
              ;

parameter: ident typedecl
         {
	     $$ = rdot_build_decl2(D_PARAMETER, $1, $2);
	 }
         ;

argument_list_stmt:
                   { $$ = NULL_DOT; }
                   | argument_list
                   {
		       $$ = grs_symbol_stack.front ();
		       grs_symbol_stack.pop_front ();
		   }
		   ;

argument_list: argument_list ',' argument
              {
		RDOT_CHAIN($1) = $3;
		$$ = $3;
	      }
              | argument
              {
		  grs_symbol_stack.push_front($1);
		  $$ = $1;
	      }
              ;

argument: expression
         ;

compound_statement: '{' '}'
                  { $$ = NULL; }
                  | '{' declaration_list '}'
                  {
		      $$ = grs_symbol_stack.front ();
		      grs_symbol_stack.pop_front ();
		  }
                  ;

declaration_list: declaration_list declaration
                {
		    RDOT_CHAIN($1) = $2;
		    $$ = $2;
		}
                | declaration
                {
		    grs_symbol_stack.push_front($1);
		    $$ = $1;
		}
	        ;

declaration: control_structure
           | var_decl ';'
           | expression ';'
           | return_stmt ';'
           | continue_stmt ';'
           | break_stmt ';'
           ;

control_structure: conditional
       | loop
       ;

loop: LOOP compound_statement
    {
	$$ = rdot_build_decl1 (D_STRUCT_LOOP, $2);
    }
    ;

conditional: if_statement elif_block else_statement
           {
	       $$ = rdot_build_decl2 (D_STRUCT_IF, $1, $3);
	       RDOT_FIELD ($$) = $2;
	   }
           ;

if_statement: IF expression compound_statement
            {
		$$ = rdot_build_decl2 (D_STRUCT_IF, $2, $3);
	    }
            ;

elif_block:
          { $$ = NULL; }
          | elif_block_list
	  {
	      $$ = grs_symbol_stack.front ();
	      grs_symbol_stack.pop_front ();
	  }
	  ;

elif_block_list: elif_block_list elif_statement
               {
		   RDOT_CHAIN($1) = $2;
		   $$ = $2;
	       }
               | elif_statement
	       {
		   grs_symbol_stack.push_front($1);
		  $$ = $1;
	       }
               ;

elif_statement: ELIF expression compound_statement
              {
		  rdot_build_decl2 (D_STRUCT_ELIF, $2, $3);
	      }
              ;

else_statement:
              { $$ = NULL; }
              | ELSE compound_statement
              {
		  $$ = rdot_build_decl1 (D_STRUCT_ELSE, $2);
	      }
              ;

return_stmt: RETURN expression
           {
             $$ = rdot_build_decl1 (C_RETURN_STMT, $2);
	   }
           | RETURN
	   {
             $$ = rdot_build_decl1 (C_RETURN_STMT, NULL_DOT);
	   }
           ;

break_stmt: BREAK
          {
	      $$ = rdot_build_decl1 (C_BREAK_STMT, NULL_DOT);
	  }
          ;

continue_stmt: CONTINUE
             {
		 $$ = rdot_build_decl1 (C_CONT_STMT, NULL_DOT);
	     }
             ;

expression: expression_stmt
          ;

target: ident
      | attributeref
      | var_decl
      ;

mutable:
       { $$ = false; }
       | MUT
       { $$ = true; }
       ;

typedecl:
        { $$ = rdot_build_decl1 (RTYPE_INFER, NULL_DOT); }
        | ':' ident
	{ $$ = $2; }
        ;

var_decl: LET mutable ident typedecl
        {
	    $$ = rdot_build_varDecl ($4, !$2, $3);
	}
        ;

expression_stmt: target '=' expression_stmt
          { $$ = rdot_build_decl2 (D_MODIFY_EXPR, $1, $3); }
          | expression_stmt '+' expression_stmt
          { $$ = rdot_build_decl2 (D_ADD_EXPR, $1, $3); }
          | expression_stmt '-' expression_stmt
          { $$ = rdot_build_decl2 (D_MINUS_EXPR, $1, $3); }
          | expression_stmt '*' expression_stmt
          { $$ = rdot_build_decl2 (D_MULT_EXPR, $1, $3); }
          | expression_stmt '%' expression_stmt
          { $$ = rdot_build_decl2 (D_MODULO_EXPR, $1, $3); }
          | expression_stmt LESS expression_stmt
          { $$ = rdot_build_decl2 (D_LESS_EXPR, $1, $3); }
          | expression_stmt GREATER expression_stmt
          { $$ = rdot_build_decl2 (D_GREATER_EXPR, $1, $3); }
          | expression_stmt EQUAL_EQUAL expression_stmt
          { $$ = rdot_build_decl2 (D_EQ_EQ_EXPR, $1, $3); }
          | expression_stmt NOT_EQUAL expression_stmt
          { $$ = rdot_build_decl2 (D_NOT_EQ_EXPR, $1, $3); }
          | '(' expression_stmt ')'
          { $$ = $2; }
          | primary
          ;

primary: atom
       | call
       | attributeref
       ;

call: primary '(' argument_list_stmt ')'
    {
      $$ = rdot_build_decl2 (D_CALL_EXPR, $1, $3);
    }
    ;

attributeref: primary '.' ident
            {
	      $$ = rdot_build_decl2 (D_ATTRIB_REF, $1, $3);
            }
	    ;

atom: struct_init
    | literal
    ;

struct_init: ident '{' struct_init_stmt '}'
           {
	       $$ = rdot_build_decl2(D_STRUCT_INIT, $1, $3);
	   }
           ;

struct_init_stmt:
                { $$ = NULL_DOT; }
                | struct_init_list
                {    
		    $$ = grs_symbol_stack.front ();
		    grs_symbol_stack.pop_front ();
		}
		;

struct_init_list: struct_init_list ',' struct_init_elem
                {
		    RDOT_CHAIN($1) = $3;
		    $$ = $3;
		}
                | struct_init_elem
		{
		    grs_symbol_stack.push_front($1);
		    $$ = $1;
		}
                ;

struct_init_elem: ident ':' expression
                {
	            $$ = rdot_build_decl2(D_STRUCT_INIT_ARGUMENT, $1, $3);
		}
                ;


literal: INTEGER
       {
	 $$ = rdot_build_integer ($1);
       }
       | STRING
       {
	 $$ = rdot_build_string ($1);
       }
       | FLOAT
       {
	   $$ = rdot_build_float($1);
       }
       | RTRUE
       {
	   $$ = rdot_build_bool(true);
       }
       | RFALSE
       {
	   $$ = rdot_build_bool(false);
       }
       | ident
       ;

ident: IDENTIFIER
     { $$ = rdot_build_identifier ($1); }
     ;

%%

void yyerror (const char* msg)
{
    fatal_error (UNKNOWN_LOCATION, "%s at line %i\n", msg, yylineno);
}
