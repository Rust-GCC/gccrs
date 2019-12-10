/* PR 30551 -Wmain is enabled by -pedantic-errors. */
/* { dg-do compile } */
/* { dg-options "-pedantic-errors" } */
void main(char a) {} /* { dg-error "first argument of .main. should be .int." "int" } */
/* { dg-error ".main. takes only zero or two arguments" "zero or two" { target *-*-* } .-1 } */ 
/* { dg-error "return type of .main. is not .int." "return type" { target *-*-* } .-2 } */ 
