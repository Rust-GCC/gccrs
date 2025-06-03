/* { dg-do compile } */
/* { dg-options "-O2 -march=cascadelake" } */

#define N 4

typedef unsigned char __vu __attribute__ ((__vector_size__ (N)));
typedef signed char __vi __attribute__ ((__vector_size__ (N)));

__vu sll (__vu a, int n)
{
  return a << n;
}

__vu sll_c (__vu a)
{
  return a << 5;
}

/* { dg-final { scan-assembler-times "vpsllw" 2 } } */

__vu srl (__vu a, int n)
{
  return a >> n;
}

__vu srl_c (__vu a)
{
  return a >> 5;
}

/* { dg-final { scan-assembler-times "vpsrlw" 2 } } */

__vi sra (__vi a, int n)
{
  return a >> n;
}

__vi sra_c (__vi a)
{
  return a >> 5;
}

/* { dg-final { scan-assembler-times "vpsraw" 2 } } */
