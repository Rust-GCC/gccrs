/* C2Y N3747 - Integer Sets, v5 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c2y -pedantic-errors -O2" } */
/* { dg-add-options float128 } */
/* { dg-add-options float64x } */
/* { dg-require-effective-target float128 } */
/* { dg-require-effective-target float64x } */

[[gnu::noipa]] void
foo (_BitInt(1) p[2], _Float128 q, _Float64x r)
{
  p[0] = q;
  p[1] = r;
}

[[gnu::noipa]] void
bar (_BitInt(1) p[2], _Float128 q[2], _Float64x r[2])
{
  q[0] = p[0];
  q[1] = p[1];
  r[0] = p[0];
  r[1] = p[1];
}

int
main ()
{
  _BitInt(1) e[10];
  foo (e, 0.5f128, 0.25f64x);
  foo (e + 2, -0.5f128, -0.25f64x);
  foo (e + 4, 0.f128, 0.f64x);
  foo (e + 6, -1.f128, -1.f64x);
  foo (e + 8, -1.5f128, -1.25f64x);
  for (int i = 0; i < 10; ++i)
    if (e[i] != (i < 6 ? 0 : -1))
      __builtin_abort ();
  e[0] = 0wb;
  e[1] = ~0wb;
  _Float128 j[2];
  _Float64x k[2];
  bar (e, j, k);
  if (j[0] != 0.0f128 || j[1] != -1.0f128
      || k[0] != 0.0f64x || k[1] != -1.0f64x)
    __builtin_abort ();
}
