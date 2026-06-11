/* { dg-do compile } */
/* { dg-options "-O2" } */

typedef signed char v16qi __attribute__((vector_size(16)));
typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

v16qi
f_qi (void)
{
  return (v16qi)
    { 3, 3, 3, 3, 3, 3, 3, 3,
      0, 0, 0, 0, 0, 0, 0, 0 };
}

v8hi
f_hi (void)
{
  return (v8hi)
    { 2, 2, 2, 2,
      0, 0, 0, 0 };
}

v4si
f_si (void)
{
  return (v4si)
    { 1, 1, 0, 0 };
}

/* { dg-final { scan-assembler-times {\tmovi\tv[0-9]+\.8b, 0x3} 1 } } */
/* { dg-final { scan-assembler-times {\tmovi\tv[0-9]+\.4h, 0x2} 1 } } */
/* { dg-final { scan-assembler-times {\tmovi\tv[0-9]+\.2s, 0x1} 1 } } */
/* { dg-final { scan-assembler-not {\tldr\tq[0-9]+,} } } */
