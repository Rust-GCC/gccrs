/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-march=armv8-a+fp16" } */

/* Check that FP vector constants with only the low element nonzero are
   materialized with scalar FMOV rather than a literal pool load.

   PR target/113856.  */

typedef float vect64_float __attribute__((vector_size(8)));
typedef float vect128_float __attribute__((vector_size(16)));
typedef _Float16 vect64_half __attribute__((vector_size(8)));
typedef _Float16 vect128_half __attribute__((vector_size(16)));
typedef double vect128_double __attribute__((vector_size(16)));

vect64_float
f1 (void)
{
  return (vect64_float) { 1.0f, 0.0f };
}

/* Existing duplicated-lane case.  */
vect64_float
f2 (void)
{
  return (vect64_float) { 1.0f, 1.0f };
}

vect128_float
f3 (void)
{
  return (vect128_float) { 1.0f, 0.0f, 0.0f, 0.0f };
}

vect64_half
f4 (void)
{
  return (vect64_half) { (_Float16) 1.0, (_Float16) 0.0,
                         (_Float16) 0.0, (_Float16) 0.0 };
}

vect128_half
f5 (void)
{
  return (vect128_half) { (_Float16) 1.0, (_Float16) 0.0,
                          (_Float16) 0.0, (_Float16) 0.0,
                          (_Float16) 0.0, (_Float16) 0.0,
                          (_Float16) 0.0, (_Float16) 0.0 };
}

vect128_double
f6 (void)
{
  return (vect128_double) { 1.0, 0.0 };
}

/* f1 and f3: scalar FMOV into the low SF element.  */
/* { dg-final { scan-assembler-times {\tfmov\ts[0-9]+, 1\.0} 2 } } */

/* f2: existing vector duplicated-FMOV case.  */
/* { dg-final { scan-assembler-times {\tfmov\tv[0-9]+\.2s, 1\.0} 1 } } */

/* f4 and f5: scalar FMOV into the low HF element.  */
/* { dg-final { scan-assembler-times {\tfmov\th[0-9]+, 1\.0} 2 } } */

/* f6: scalar FMOV into the low DF element.  */
/* { dg-final { scan-assembler-times {\tfmov\td[0-9]+, 1\.0} 1 } } */

/* None of them should need a literal pool load.  */
/* { dg-final { scan-assembler-not {\tldr\tq[0-9]+,} } } */
