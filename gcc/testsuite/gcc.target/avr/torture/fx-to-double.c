/* { dg-do run { target { ! avr_tiny } } } */
/* { dg-additional-options "-std=gnu99 -Wno-pedantic" } */

typedef long double DD;

typedef long long _Fract dq_t;
typedef long long _Accum ta_t;
typedef long _Accum da_t;
typedef unsigned long long _Fract udq_t;
typedef unsigned long long _Accum uta_t;
typedef unsigned long _Accum uda_t;

#define NI __attribute__((noipa))

#define MK_CONV(T)				\
  NI DD T##2d (T##_t x)				\
  {						\
    __asm ("" : "+r" (x));			\
    return (DD) x;				\
  }						\
						\
  NI T##_t d2##T (DD x)				\
  {						\
    __asm ("" : "+r" (x));			\
    return (T##_t) x;				\
  }


MK_CONV (da)
MK_CONV (ta)
MK_CONV (dq)

MK_CONV (uda)
MK_CONV (uta)
MK_CONV (udq)

#define TEST(T)					\
  do {						\
    T##_t fx = d2##T (x);			\
    DD y = T##2d (fx);				\
    if (y != x)					\
      __builtin_exit (__LINE__);		\
  } while (0)


NI void test_s (DD x)
{
  TEST (da);
  TEST (ta);

  if (x < 1.0L && x >= -1.0L)
    TEST (dq);
}


NI void test_u (long double x)
{
  TEST (uda);
  TEST (uta);

  if (x < 1.0L)
    TEST (udq);
}

void test (DD x)
{
  if (x >= 0)
    test_u (x);

  test_s (x);
}

int main (void)
{
  test (+0.0L);
  test (-0.0L);

  test (+1.0L);
  test (-1.0L);

  test (-0x0.cafe123p0);
  test (+0x0.cafe123p0);

  test (+0x1234.cafe123p0);
  test (-0x1234.cafe123p0);

  return 0;
}
