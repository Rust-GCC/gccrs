/* PR tree-optimization/124988 */
/* { dg-do run { target bitint } } */

int a;
#if __BITINT_MAXWIDTH__ >= 192
unsigned _BitInt(132) b;

unsigned _BitInt(128)
foo (unsigned _BitInt(192) d)
{
  bool o = __builtin_mul_overflow (d, 1, &b);
  if (!o)
    __atomic_sub_fetch (&a, 1, 0);
  return b >> 8;
}
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 192
  unsigned _BitInt(128) x = foo (0x1100ffeeddccbbaa998877665544332211wb);
  if (x != 0x100ffeeddccbbaa9988776655443322wb)
    __builtin_abort ();
#endif
}
