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
#include "y.rs.h"

extern int yyparse (void);

%}

%option noyywrap
%option yylineno

DIGIT        [0-9]
ID           [_a-zA-Z][a-zA_Z0-9_$]*
QSTRING      \"[^\"\n]*[\"\n]

%%

fn                      {  return DEFUN; }
let                     {  return LET; }
mut                     {  return MUT; }
while                   {  return WHILE; }
loop                    {  return LOOP; }
static                  {  return STATIC; }

bool                    {  return TYPE_BOOL; }
int                     {  return TYPE_INT; }
float                   {  return TYPE_FLOAT; }
uint                    {  return TYPE_UINT; }

\[                      {  return '['; }
\]                      {  return ']'; }
\(                      {  return '('; }
\)                      {  return ')'; }
\{                      {  return '{'; }
\}                      {  return '}'; }

"->"                    {  return RTYPE; }

";"                     { return ';'; }
","                     { return ','; }
"."                     { return '.'; }
":"                     { return ':'; }

"="                     { return '='; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"/"                     { return '/'; }
"*"                     { return '*'; }
"|"                     { return '|'; }

"=="                    { return EQUAL_EQUAL; }

"!="                    { return NOT_EQUAL; }
"<"                     { return LESS; }
"<="                    { return LESS_EQUAL; }
">"                     { return GREATER; }
">="                    { return GREATER_EQUAL; }

{QSTRING}               {
  yylval.string = xstrdup (yytext + 1);
  if (yylval.string [yyleng - 2] != '\"') {
    error ("Un-termintated character string!\n");
  }
  else {
    yylval.string [yyleng - 2] = '\0';
  }
  return STRING;
}

{DIGIT}+                {
  mpfr_t x;
  mpfr_init2 (x, 32);
  if (mpfr_set_str (x, yytext, 10, GMP_RNDU))
    {
      fatal_error ("error initilizing integer value <%s>!\n", yytext);
    }
  yylval.integer = mpfr_get_si (x, GMP_RNDU);
  mpfr_clear (x);
  return INTEGER;
       }

{ID}                    {
  yylval.string = xstrdup (yytext);
  return IDENTIFIER;
}

.           {  }
\t          {  }
\n          { yylineno++; }

%%

bool grs_do_compile (const char * in)
{
    bool retval = true;
    FILE * fd = fopen (in, "rb");
    if (fd)
    {
	yyin = fd;
	retval = yyparse ();
	fclose (fd);
	yylex_destroy ();
    }
    else
    {
	fprintf (stderr, "error opening file %s\n", in);
	retval = false;
    }
    return retval;
}
