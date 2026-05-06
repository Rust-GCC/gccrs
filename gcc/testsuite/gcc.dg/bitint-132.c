/* C2Y N3747 - Integer Sets, v5 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c2y -pedantic-errors -O2" } */
/* { dg-add-options float16 } */
/* { dg-require-effective-target float16_runtime } */

[[gnu::noipa]] _BitInt(1)
foo (_Float16 x)
{
  return x;
}

[[gnu::noipa]] _Float16
bar (_BitInt(1) x)
{
  return x;
}

int
main ()
{
  if (foo (0.5f16) != 0wb || foo (0.25f16) != 0wb
      || foo (-0.5f16) != 0wb || foo (-0.25f16) != 0wb
      || foo (0.f16) != 0wb || foo (-0.75f16) != 0wb
      || foo (-1.f16) != ~0wb || foo (-1.75f16) != ~0wb
      || foo (-1.5f16) != ~0wb || foo (-1.25f16) != ~0wb)
    __builtin_abort ();
  if (bar (0wb) != 0.f16 || bar (~0wb) != -1.f16)
    __builtin_abort ();
}
