/* { dg-do run { target { ! avr_tiny } } } */
/* { dg-options "-std=gnu99 -fwrapv" } */

#include "fix-types.h"

extern void abort (void);
extern void exit (int);

typedef _Fract fx_t;
typedef _Sat _Fract satfx_t;
typedef int intfx_t;

SS_FUN (ss_add, +, fx_t, r)
SS_FUN (ss_sub, -, fx_t, r)

#define VAL(N, X)                               \
  __attribute__((noipa))                        \
  satfx_t ss_add2_##N (satfx_t a)               \
  {                                             \
    return ss_add_r (a, X##P##-##15r);          \
  }                                             \
  __attribute__((noipa))                        \
  satfx_t ss_add_##N (satfx_t a)                \
  {                                             \
    return a + X##P##-##15r;                    \
  }                                             \
  __attribute__((noipa))                        \
  satfx_t ss_sub2_##N (satfx_t a)               \
  {                                             \
    return ss_sub_r (a, X##P##-##15r);          \
  }                                             \
  __attribute__((noipa))                        \
  satfx_t ss_sub_##N (satfx_t a)                \
  {                                             \
    return a - X##P##-##15r;                    \
  }
#include "vals-r.def"
#undef VAL

__attribute__((noipa))
satfx_t ss_add2_99 (satfx_t a)
{
  return ss_add_r (a, __FRACT_MIN__);
}

__attribute__((noipa))
satfx_t ss_add_99 (satfx_t a)
{
  return a + __FRACT_MIN__;
}

__attribute__((noipa))
satfx_t ss_sub2_99 (satfx_t a)
{
  return ss_sub_r (a, __FRACT_MIN__);
}

__attribute__((noipa))
satfx_t ss_sub_99 (satfx_t a)
{
  return a - __FRACT_MIN__;
}


satfx_t (* __flash const fun[])(satfx_t) =
{
#define VAL(N, X)                               \
  ss_add_##N, ss_add2_##N,                      \
  ss_sub_##N, ss_sub2_##N,
#include "vals-r.def"
  VAL (99,)
#undef VAL
};


const volatile __flash intfx_t vals[] =
  {
    0, -1, 1, -2, 2, -127, -128, -129,
    0x7f, 0x80, 0x81, 0x100,
    0x4000, 0x3e80, 0x3f80,
    0x7ffe, 0x7fff,
    0x7f7f, 0x7f81, 0x7f80,
    0x7f01,
    0x8000, 0x8001, 0x8080,
    0x8081, 0x80ff, 0x80fe,
    0x8100, 0x8180, 0x817f,
    0xff00, 0xff01, 0xff01,
    0xff7f, 0xff80
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
