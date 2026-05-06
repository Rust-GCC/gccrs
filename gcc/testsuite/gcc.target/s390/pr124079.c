/* PR rtl-optimization/124079 */
/* { dg-do compile } */
/* { dg-options "-O2 -march=z13" } */

int foo (const char *a, const char *b)
{
  return __builtin_strcmp (a, b);
}
