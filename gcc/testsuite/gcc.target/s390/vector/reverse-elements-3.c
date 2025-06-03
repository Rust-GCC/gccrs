/* { dg-do compile } */
/* { dg-options "-O3 -mzarch -march=z14" } */
/* { dg-require-effective-target s390_vxe } */
/* { dg-final { scan-assembler-times {\tvpdi\t} 5 } } */
/* { dg-final { scan-assembler-not {\tvperm\t} } } */

typedef short __attribute__ ((vector_size (16))) V8HI;
typedef int __attribute__ ((vector_size (16))) V4SI;
typedef long long __attribute__ ((vector_size (16))) V2DI;
typedef float __attribute__ ((vector_size (16))) V4SF;
typedef double __attribute__ ((vector_size (16))) V2DF;

V8HI
v8hi (V8HI *x)
{
  V8HI y;
  for (int i = 0; i < 8; ++i)
    y[i] = (*x)[7 - i];
  return y;
}

V4SI
v4si (V4SI *x)
{
  V4SI y;
  for (int i = 0; i < 4; ++i)
    y[i] = (*x)[3 - i];
  return y;
}

V2DI
v2di (V2DI *x)
{
  V2DI y;
  for (int i = 0; i < 2; ++i)
    y[i] = (*x)[1 - i];
  return y;
}

V4SF
v4sf (V4SF *x)
{
  V4SF y;
  for (int i = 0; i < 4; ++i)
    y[i] = (*x)[3 - i];
  return y;
}

V2DF
v2df (V2DF *x)
{
  V2DF y;
  for (int i = 0; i < 2; ++i)
    y[i] = (*x)[1 - i];
  return y;
}
