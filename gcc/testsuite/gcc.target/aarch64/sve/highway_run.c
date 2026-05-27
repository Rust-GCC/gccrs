/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-require-effective-target lp64 } */
/* { dg-options "-O2" } */

#include <arm_sve.h>

extern void abort (void) __attribute__ ((noreturn));

volatile int cond = 1;

int __attribute__ ((noipa))
a (void)
{
  return cond;
}

#define TEST_LOAD(TYPE, NAME, BITS)					\
  int __attribute__ ((noipa))						\
  test_##NAME (void)							\
  {									\
    TYPE *g = __builtin_malloc (sizeof (TYPE));				\
    int c = 0;								\
    if (!g)								\
      abort ();								\
    g[0] = 0;								\
    if (a ())								\
      {									\
	g[0] = 1;							\
	c = 2;								\
      }									\
    svint##BITS##_t d						\
      = svld1_s##BITS (svptrue_pat_b##BITS (SV_VL1), g);		\
    svbool_t e = svcmpgt_s##BITS (svptrue_b##BITS (), d,		\
				  svdup_n_s##BITS (0));		\
    int f = svptest_any (svptrue_pat_b##BITS (SV_VL1), e);		\
    if (f && c != 2)							\
      abort ();								\
    return f;								\
  }

TEST_LOAD (signed char, byte, 8)
TEST_LOAD (short, short, 16)
TEST_LOAD (int, int, 32)
TEST_LOAD (long, long, 64)

int
main (void)
{
  if (!test_byte ()
      || !test_short ()
      || !test_int ()
      || !test_long ())
    abort ();
  return 0;
}
