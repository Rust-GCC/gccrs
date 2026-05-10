/* { dg-do run } */
/* { dg-options "-O0 -march=x86-64-v3" } */

void
ext (long a, long b, long c, long d, long e, long f, long g)
{
}

void
tail (long a, long b)
{
}

__attribute__ ((preserve_none)) void
caller (long a, long b, long c, long d, long e, long f, long g)
{
  __attribute__ ((aligned (32))) long x[4]; // for stack alignment
  ext (a, b, c, d, e, f, g);
  __attribute__ ((musttail)) return tail (a + b, b + c);
}

__attribute__ ((noipa))
static void
do_test ()
{
  caller (1, 2, 3, 4, 5, 6, 7);
}

int
main (void)
{
  if (__builtin_cpu_supports ("x86-64-v3"))
    do_test ();
  return 0;
}
