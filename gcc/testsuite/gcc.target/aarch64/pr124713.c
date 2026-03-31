/* PR rtl-optimization/124713 */
/* { dg-do run } */
/* { dg-require-effective-target aarch64_little_endian } */
/* { dg-options "-Og -favoid-store-forwarding -fwrapv" } */

int x;
long y;

__attribute__((noipa)) long
foo (short m)
{
  short b = m * 13;
  __builtin_memset (1 + (char *) &b, x, 1);
  long c = *(long *) __builtin_memset (&y, x, 4);
  return c + b;
}

int
main ()
{
  long x = foo (3840);
  if (x)
    __builtin_abort ();
}
