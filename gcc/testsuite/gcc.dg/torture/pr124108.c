/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */

typedef unsigned _BitInt(4) B;

B a, b;

void
foo()
{
  b *= (a ^ 2wbu) << 2 |
       (a ^ 2wbu) >> 2;
}
