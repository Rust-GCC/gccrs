/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=future -O2 -ftree-vectorize -fno-unroll-loops" } */
/* { dg-require-effective-target powerpc_future_compile_ok } */

#define N 128

extern signed int   si_a[N],  si_b[N],  si_c[N];
extern unsigned int ui_a[N],  ui_b[N],  ui_c[N];
extern signed short ss_a[N],  ss_b[N],  ss_c[N];
extern unsigned short us_a[N], us_b[N], us_c[N];

void int_mulhi ()
{
  for (int i = 0; i < N; i++)
    si_c[i] = ((signed long long) si_a[i] *
               (signed long long) si_b[i]) >> 32;
}

void uint_mulhi ()
{
  for (int i = 0; i < N; i++)
    ui_c[i] = ((unsigned long long) ui_a[i] *
               (unsigned long long) ui_b[i]) >> 32;
}

void short_mulhi ()
{
  for (int i = 0; i < N; i++)
    ss_c[i] = ((signed int) ss_a[i] *
               (signed int) ss_b[i]) >> 16;
}

void ushort_mulhi ()
{
  for (int i = 0; i < N; i++)
    us_c[i] = ((unsigned int) us_a[i] *
               (unsigned int) us_b[i]) >> 16;
}

/* { dg-final { scan-assembler-times {\mxvmulhsw\M} 1 } } */
/* { dg-final { scan-assembler-times {\mxvmulhuw\M} 1 } } */
/* { dg-final { scan-assembler-times {\mxvmulhsh\M} 1 } } */
/* { dg-final { scan-assembler-times {\mxvmulhuh\M} 1 } } */
