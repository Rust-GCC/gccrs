/* PR rtl-optimization/79909 */
/* { dg-do compile } */
/* { dg-options "-O2 -mxl-compat" } */
/* { dg-require-effective-target dfp } */
/* { dg-skip-if "DFP" { powerpc*-*-aix* } } */

typedef float T __attribute__ ((mode (TD)));
T b, c, d, e, f, g;
void bar (T, T, T, T, T, T);

void
foo (void)
{
  bar (e, b, f, c, g, d);
}
