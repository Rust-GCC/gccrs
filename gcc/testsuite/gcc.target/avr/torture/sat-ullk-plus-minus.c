/* { dg-do run { target { ! avr_tiny } } } */
/* { dg-options "-std=gnu99 -fwrapv" } */

#include "fix-types.h"

extern void abort (void);
extern void exit (int);

typedef unsigned long long _Accum fx_t;
typedef unsigned long long _Sat _Accum satfx_t;
typedef unsigned long long intfx_t;

US_LFUN (us_add, +, fx_t, ullk, >)
US_LFUN (us_sub, -, fx_t, ullk, <)

#define VAL(N, X)                               \
  __attribute__((noipa))                        \
  satfx_t us_add2_##N (satfx_t a)               \
  {                                             \
    return us_add_ullk (a, X##P##-##48ullk);    \
  }                                             \
  __attribute__((noipa))                        \
  satfx_t us_add_##N (satfx_t a)                \
  {                                             \
    return a + X##P##-##48ullk;                 \
  }                                             \
  __attribute__((noipa))                        \
  satfx_t us_sub2_##N (satfx_t a)               \
  {                                             \
    return us_sub_ullk (a, X##P##-##48ullk);    \
  }                                             \
  __attribute__((noipa))                        \
  satfx_t us_sub_##N (satfx_t a)                \
  {                                             \
    return a - X##P##-##48ullk;                 \
  }
#include "vals-ullk.def"
#undef VAL

satfx_t (* __flash const fun[])(satfx_t) =
{
#define VAL(N, X)                               \
  us_add_##N, us_add2_##N,                      \
  us_sub_##N, us_sub2_##N,
#include "vals-ullk.def"
#undef VAL
};


const volatile __flash intfx_t vals[] =
  {
    0, -1, 1, -2, 2, -127, -128, -129,
    0x7f, 0x80, 0x81, 0x100,
    0x4000000000000000, 0x3e80000000000000, 0x3f80000000000000,
    0x7ffffffffffffffe, 0x7fffffffffffffff, 0x7f80000000000000,
    0x7f7f7f7f7f7f7f7f, 0x7f81000000000080, 0x7f00000080000000,
    0x7f00000000000001,
    0x8000000000000000, 0x8000000000000001, 0x8080808080808080,
    0x8081000000000000, 0x80ffffffffffffff, 0x80fffffffffffffe,
    0x8100000000000000, 0x8180000000000000, 0x818000000000000,
    0xff00000000000000, 0xffffffffffffff01, 0xffffffffffffff80,
    0xffffffffffffff7f, 0xff80ff80ff80ff80
  };


int main (void)
{
  for (unsigned int i = 0; i < sizeof (vals) / sizeof (*vals); i++)
    {
      satfx_t a, f1, f2;
      intfx_t val = vals[i];
      __builtin_memcpy (&a, &val, sizeof (satfx_t));
      for (unsigned int f = 0; f < sizeof (fun) / sizeof (*fun); f += 2)
        {
          if (fun[f](a) != fun[f+1](a))
            abort();
        }
    }

  exit (0);
  return 0;
}
