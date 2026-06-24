/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=future -O2" } */
/* { dg-require-effective-target powerpc_future_compile_ok } */

#include <altivec.h>

typedef vector signed int      vsi;
typedef vector unsigned int    vui;
typedef vector signed short    vss;
typedef vector unsigned short  vus;

vsi int_add (vsi x, vsi y)
{
  /* force x & y into FPRs to prefer xvadduwm over vadduwm.  */
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x + y;            /* xvadduwm */
}

vsi int_sub (vsi x, vsi y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x - y;            /* xvsubuwm */
}

vsi int_mul (vsi x, vsi y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x * y;            /* xvmuluwm */
}

vui uint_add (vui x, vui y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x + y;            /* xvadduwm */
}

vui uint_sub (vui x, vui y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x - y;            /* xvsubuwm */
}

vui uint_mul (vui x, vui y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x * y;            /* xvmuluwm */
}

vss short_add (vss x, vss y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x + y;            /* xvadduhm */
}

vss short_sub (vss x, vss y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x - y;            /* xvsubuhm */
}

vss short_mul (vss x, vss y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x * y;            /* xvmuluhm */
}

vus ushort_add (vus x, vus y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x + y;            /* xvadduhm */
}

vus ushort_sub (vus x, vus y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x - y;            /* xvsubuhm */
}

vus ushort_mul (vus x, vus y)
{
  __asm__ (" # %x0,%x0" : "+f" (x), "+f" (y));
  return x * y;            /* xvmuluhm */
}

/* { dg-final { scan-assembler-times "xvadduwm" 2 } } */
/* { dg-final { scan-assembler-times "xvsubuwm" 2 } } */
/* { dg-final { scan-assembler-times "xvmuluwm" 2 } } */
/* { dg-final { scan-assembler-times "xvadduhm" 2 } } */
/* { dg-final { scan-assembler-times "xvsubuhm" 2 } } */
/* { dg-final { scan-assembler-times "xvmuluhm" 2 } } */
