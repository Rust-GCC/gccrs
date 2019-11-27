%{
#include "rdot.h"
#include "rs-parser.h"

#include <gmp.h>
#include <mpfr.h>

%}

DIGIT        [0-9]
ID           [_a-zA-Z][a-zA_Z0-9_$]*
QSTRING      \"[^\"\n]*[\"\n]

%option nodefault
%option noyywrap
%option yylineno

%x comment
%%

"/*"                    {  BEGIN (comment); }
<comment>[^*\n]*        {  /* eat it up flex you be le hungry maybe? */ }
<comment>"*"+[^*/\n]*   {  /* eat up boyo */ }
<comment>\n             {  ++yylineno; }
<comment>"*"+"/"        {  BEGIN (INITIAL); }

pub                     { return PUB; }
break                   { return BREAK; }
return                  { return RETURN; }
continue                { return CONTINUE; }
fn                      { return FN; }
let                     { return LET; }
mut                     { return MUT; }
loop                    { return LOOP; }
static                  { return STATIC; }
struct                  { return STRUCT; }
if                      { return IF; }
"else if"               { return ELIF; }
else                    { return ELSE; }
impl                    { return IMPL; }
self                    { return SELF; }
true                    { return RTRUE; }
false                   { return RFALSE; }
\[                      { return '['; }
\]                      { return ']'; }
\(                      { return '('; }
\)                      { return ')'; }
\{                      { return '{'; }
\}                      { return '}'; }
"->"                    { return POINTER; }
";"                     { return ';'; }
","                     { return ','; }
"."                     { return '.'; }
":"                     { return ':'; }
"::"                    { return NS_ACC; }
"="                     { return '='; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"/"                     { return '/'; }
"*"                     { return '*'; }
"|"                     { return '|'; }
"~"                     { return '~'; }
"&"                     { return '&'; }
"=="                    { return EQUAL_EQUAL; }
"!="                    { return NOT_EQUAL; }
"<"                     { return '<'; }
"<="                    { return LESS_EQUAL; }
">"                     { return '>'; }
">="                    { return GREATER_EQUAL; }
"%"                     { return '%'; }

\/\/.*                  ;  /* // style comment */

{DIGIT}+                {
  mpfr_t x;
  mpfr_init2 (x, 32);
  if (mpfr_set_str (x, yytext, 10, GMP_RNDU))
    {
      fatal_error (UNKNOWN_LOCATION, "error initilizing integer value <%s>!\n", yytext);
    }
  yylval.integer = mpfr_get_si (x, GMP_RNDU);
  mpfr_clear (x);
  return INTEGER;
}

{DIGIT}+"."{DIGIT}+    {
  mpfr_t x;
  mpfr_init2 (x, 32);
  if (mpfr_set_str (x, yytext, 10, GMP_RNDU))
    {
      fatal_error (UNKNOWN_LOCATION, "error initilizing float value <%s>!\n", yytext);
    }
  yylval.decimal = mpfr_get_flt (x, GMP_RNDU);
  mpfr_clear (x);
  return FLOAT;
}

{ID}                    {
  yylval.str = xstrdup(yytext);
  return IDENTIFIER;
}

.           {  }
\n          {  }

%%

