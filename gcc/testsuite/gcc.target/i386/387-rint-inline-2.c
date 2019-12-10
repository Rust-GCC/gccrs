/* Test rint and related functions expanded inline for 387.  rint
   should be expanded even when spurious "inexact" not allowed.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfancy-math-387 -mfpmath=387 -fno-fp-int-builtin-inexact" } */

#define TEST(FN, TYPE)				\
  do						\
    {						\
      volatile TYPE a = 1.5, b;			\
      b = FN (a);				\
    }						\
  while (0)

#define FN_TESTS(FN)					\
  do							\
    {							\
      TEST (__builtin_ ## FN, double);			\
      TEST (__builtin_ ## FN ## f, float);		\
      TEST (__builtin_ ## FN ## l, long double);	\
    }							\
  while (0)

void
test (void)
{
  FN_TESTS (rint);
}

/* { dg-final { scan-assembler-not "\[ \t\]rint" } } */
