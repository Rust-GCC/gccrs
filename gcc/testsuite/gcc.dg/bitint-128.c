/* PR tree-optimization/124826 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */

typedef float V __attribute__((vector_size (16 * sizeof (float))));

union {
#if __BITINT_MAXWIDTH__ >= 256
  _BitInt(256) b;
#endif
  V v;
} u;

void
foo (int c)
{
  u.v -= c ? 0.f : 1.f;
#if __BITINT_MAXWIDTH__ >= 256
  u.b *= c;
#endif
}

int
main ()
{
  foo (2);
}
