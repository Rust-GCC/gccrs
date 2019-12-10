/* PR rtl-optimization/36017 */
/* { dg-do run } */
/* { dg-options "-O2" } */

#if defined (__AVR__) && (__SIZEOF_DOUBLE__ == __SIZEOF_FLOAT__)
extern double sqrt (double) __asm ("sqrtf");
#else
extern double sqrt (double);
#endif
extern void abort (void);

__attribute__((noinline)) double
foo (double a)
{
  double b, c, d = 0.7;
  if (a <= d)
    b = sqrt (d * a);
  else
    {
      c = (1.0 - d) * (1.0 - a);
      b = c > 0 ? 1.0 - sqrt (c) : 1.0;
    }
  return b;
}

int
main (void)
{
  double c = foo (0.5);
  if (c > 0.5917)
    abort ();
  return 0;
}
