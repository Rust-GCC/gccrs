/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=future -O2" } */
/* { dg-require-effective-target powerpc_future_compile_ok } */

#include <altivec.h>

typedef vector signed int   v4si_t;
typedef vector signed short v8hi_t;
typedef vector signed long long v2di_t;

v4si_t int_add (v4si_t x, v4si_t y)
{
  /* force x & y into FPRs to prefer xvadduwm over vadduwm.  */
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_add (x, y);            /* xvadduwm */
}

v4si_t int_sub (v4si_t x, v4si_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_sub (x, y);            /* xvsubuwm */
}

v4si_t int_mul (v4si_t x, v4si_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mul (x, y);            /* xvmuluwm */
}

v4si_t int_mulhi (v4si_t x, v4si_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mulh (x, y);           /* xvmulhsw */
}

v8hi_t short_add (v8hi_t x, v8hi_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_add (x, y);            /* xvadduhm */
}

v8hi_t short_sub (v8hi_t x, v8hi_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_sub (x, y);            /* xvsubuhm */
}

v8hi_t short_mul (v8hi_t x, v8hi_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mul (x, y);            /* xvmuluhm */
}

v8hi_t short_mulhi (v8hi_t x, v8hi_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mulh (x, y);            /* xvmulhsh */
}

v2di_t longlong_mulhi (v2di_t x, v2di_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mulh (x, y);            /* vmulhsd */
}

/* { dg-final { scan-assembler-times "xvadduwm" 1 } } */
/* { dg-final { scan-assembler-times "xvsubuwm" 1 } } */
/* { dg-final { scan-assembler-times "xvmuluwm" 1 } } */
/* { dg-final { scan-assembler-times "xvmulhsw" 1 } } */
/* { dg-final { scan-assembler-times "xvadduhm" 1 } } */
/* { dg-final { scan-assembler-times "xvsubuhm" 1 } } */
/* { dg-final { scan-assembler-times "xvmuluhm" 1 } } */
/* { dg-final { scan-assembler-times "xvmulhsh" 1 } } */
/* { dg-final { scan-assembler-times "vmulhsd" 1 } } */
