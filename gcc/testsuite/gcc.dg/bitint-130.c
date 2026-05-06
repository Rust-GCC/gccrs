/* C2Y N3747 - Integer Sets, v5 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c2y -pedantic-errors -O2" } */
/* { dg-add-options float32 } */
/* { dg-add-options float64 } */
/* { dg-add-options float32x } */
/* { dg-require-effective-target float32 } */
/* { dg-require-effective-target float32x } */
/* { dg-require-effective-target float64 } */

[[gnu::noipa]] void
foo (_BitInt(1) p[3], _Float32 q, _Float64 r, _Float32x s)
{
  p[0] = q;
  p[1] = r;
  p[2] = s;
}

[[gnu::noipa]] void
bar (_BitInt(1) p[2], _Float32 q[2], _Float64 r[2], _Float32x s[2])
{
  q[0] = p[0];
  q[1] = p[1];
  r[0] = p[0];
  r[1] = p[1];
  s[0] = p[0];
  s[1] = p[1];
}

int
main ()
{
  _BitInt(1) e[15];
  foo (e, 0.5f32, 0.25f64, 0.75f32x);
  foo (e + 3, -0.5f32, -0.25f64, -0.75f32x);
  foo (e + 6, 0.f32, 0.f64, 0.f32x);
  foo (e + 9, -1.f32, -1.f64, -1.f32x);
  foo (e + 12, -1.5f32, -1.25f64, -1.75f32x);
  for (int i = 0; i < 15; ++i)
    if (e[i] != (i < 9 ? 0 : -1))
      __builtin_abort ();
  e[0] = 0wb;
  e[1] = ~0wb;
  _Float32 j[2];
  _Float64 k[2];
  _Float32x l[2];
  bar (e, j, k, l);
  if (j[0] != 0.0f32 || j[1] != -1.0f32
      || k[0] != 0.0f64 || k[1] != -1.0f64
      || l[0] != 0.0f32x || l[1] != -1.0f32x)
    __builtin_abort ();
}
