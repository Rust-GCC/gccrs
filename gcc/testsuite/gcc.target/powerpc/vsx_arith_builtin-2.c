/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=future -O2" } */
/* { dg-require-effective-target powerpc_future_compile_ok } */

#include <altivec.h>

typedef vector unsigned int   v4si_t;
typedef vector unsigned short v8hi_t;
typedef vector unsigned long long v2di_t;

v4si_t uint_add (v4si_t x, v4si_t y)
{
  /* force x & y into FPRs to prefer xvadduwm over vadduwm.  */
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_add (x, y);            /* xvadduwm */
}

v4si_t uint_sub (v4si_t x, v4si_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_sub (x, y);            /* xvsubuwm */
}

v4si_t uint_mul (v4si_t x, v4si_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mul (x, y);            /* xvmuluwm */
}

v4si_t uint_mulhi (v4si_t x, v4si_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mulh (x, y);           /* xvmulhuw */
}

v8hi_t ushort_add (v8hi_t x, v8hi_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_add (x, y);            /* xvadduhm */
}

v8hi_t ushort_sub (v8hi_t x, v8hi_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_sub (x, y);            /* xvsubuhm */
}

v8hi_t ushort_mul (v8hi_t x, v8hi_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mul (x, y);            /* xvmuluhm */
}

v8hi_t ushort_mulhi (v8hi_t x, v8hi_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mulh (x, y);            /* xvmulhuh */
}

v2di_t ulonglong_mulhi (v2di_t x, v2di_t y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return vec_mulh (x, y);            /* vmulhud */
}

/* { dg-final { scan-assembler-times "xvadduwm" 1 } } */
/* { dg-final { scan-assembler-times "xvsubuwm" 1 } } */
/* { dg-final { scan-assembler-times "xvmuluwm" 1 } } */
/* { dg-final { scan-assembler-times "xvmulhuw" 1 } } */
/* { dg-final { scan-assembler-times "xvadduhm" 1 } } */
/* { dg-final { scan-assembler-times "xvsubuhm" 1 } } */
/* { dg-final { scan-assembler-times "xvmuluhm" 1 } } */
/* { dg-final { scan-assembler-times "xvmulhuh" 1 } } */
/* { dg-final { scan-assembler-times "vmulhud" 1 } } */
