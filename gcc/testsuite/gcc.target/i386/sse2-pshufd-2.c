/* { dg-do compile } */
/* { dg-options "-msse2 -O2" } */

typedef int __v4si __attribute__ ((__vector_size__ (16)));

__v4si foo(__v4si x)
{
  __v4si t = __builtin_ia32_pshufd (x, 225);
  return __builtin_ia32_pshufd (t, 198);
}

/* { dg-final { scan-assembler-times "pshufd\[ \\t\]+" 1 } } */
