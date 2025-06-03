/* PR c/115290 */
/* { dg-do compile } */
/* { dg-options "-Warray-compare" } */

int a[32][32], b[32][32];

int
foo (int x, int y)
{
  return (x ? a : b) == (y ? a : b); /* { dg-warning "comparison between two arrays" "" { target { c || c++23_down } } } */
/* { dg-error "comparison between two arrays" "" { target c++26 } .-1 } */
/* { dg-message "use '&\\\(\[^\n\r]*\\\)\\\[0\\\] == &\\\(\[^\n\r]*\\\)\\\[0\\\]' to compare the addresses" "" { target c } .-2 } */
/* { dg-message "use unary '\\\+' which decays operands to pointers or '&\\\(\[^\n\r]*\\\)\\\[0\\\] == &\\\(\[^\n\r]*\\\)\\\[0\\\]' to compare the addresses" "" { target c++ } .-3 } */
}
