/* PR tree-optimization/124941 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -fno-strict-aliasing" } */

#if __BITINT_MAXWIDTH__ >= 1024
_BitInt(257)
foo ()
{
  _BitInt(1024) b = 0;
  return *(_BitInt(257) *)&b;
}
#else
int i;
#endif
