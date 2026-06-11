/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8.2-a+fp16" } */

typedef float  v2sf  __attribute__((vector_size(8)));
typedef float  v4sf  __attribute__((vector_size(16)));
typedef double v2df  __attribute__((vector_size(16)));
typedef __fp16 v4hf  __attribute__((vector_size(8)));
typedef __fp16 v8hf  __attribute__((vector_size(16)));

v2sf
f_v2sf (void)
{
  return (v2sf){ 1.0f, 1.0f };
}

v4sf
f_v4sf (void)
{
  return (v4sf){ 1.0f, 1.0f, 0.0f, 0.0f };
}

v2df
f_v2df (void)
{
  return (v2df){ 1.0, 0.0 };
}

v4hf
f_v4hf (void)
{
  return (v4hf){ (__fp16)1.0, (__fp16)1.0, (__fp16)1.0, (__fp16)1.0 };
}

v8hf
f_v8hf (void)
{
  return (v8hf){ (__fp16)1.0, (__fp16)1.0, (__fp16)1.0, (__fp16)1.0, 0, 0, 0, 0 };
}

/* Each function should use fmov, not a literal pool load.  */
/* { dg-final { scan-assembler-times {fmov[ \t]+v[0-9]+[.]2s,[ \t]+1[.]0e[+]0} 2 } } */
/* { dg-final { scan-assembler-times {fmov[ \t]+v[0-9]+[.]4h,[ \t]+1[.]0e[+]0} 2 } } */
/* { dg-final { scan-assembler-times {\tfmov\td[0-9]+,} 1 } } */
/* { dg-final { scan-assembler-not  {\tldr\t[sdq][0-9]+, =} } } */
