/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -fno-tree-vectorize" } */

#include "riscv_vector.h"

void f (int8_t * restrict in, int8_t * restrict out, int n)
{
  if (n == 0) {
    vbool64_t v = *(vbool64_t*)(in + 500);
    *(vbool64_t*)(out + 500) = v;
  } else if (n == 1) {
    vbool64_t v = *(vbool64_t*)(in + 600);
    *(vbool64_t*)(out + 600) = v;
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vbool64_t v = *(vbool64_t*)(in + 900 + i);
    *(vbool64_t*)(out + 900 + i) = v;
  }
}

void f2 (int8_t * restrict in, int8_t * restrict out, int n)
{
  if (n == 0) {
    vbool32_t v = *(vbool32_t*)(in + 500);
    *(vbool32_t*)(out + 500) = v;
  } else if (n == 1) {
    vbool32_t v = *(vbool32_t*)(in + 600);
    *(vbool32_t*)(out + 600) = v;
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vbool32_t v = *(vbool32_t*)(in + 900 + i);
    *(vbool32_t*)(out + 900 + i) = v;
  }
}

void f3 (int8_t * restrict in, int8_t * restrict out, int n)
{
  if (n == 0) {
    vbool16_t v = *(vbool16_t*)(in + 500);
    *(vbool16_t*)(out + 500) = v;
  } else if (n == 1) {
    vbool16_t v = *(vbool16_t*)(in + 600);
    *(vbool16_t*)(out + 600) = v;
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vbool16_t v = *(vbool16_t*)(in + 900 + i);
    *(vbool16_t*)(out + 900 + i) = v;
  }
}

void f4 (int8_t * restrict in, int8_t * restrict out, int n)
{
  if (n == 0) {
    vbool8_t v = *(vbool8_t*)(in + 500);
    *(vbool8_t*)(out + 500) = v;
  } else if (n == 1) {
    vbool8_t v = *(vbool8_t*)(in + 600);
    *(vbool8_t*)(out + 600) = v;
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vbool8_t v = *(vbool8_t*)(in + 900 + i);
    *(vbool8_t*)(out + 900 + i) = v;
  }
}

void f5 (int8_t * restrict in, int8_t * restrict out, int n)
{
  if (n == 0) {
    vbool4_t v = *(vbool4_t*)(in + 500);
    *(vbool4_t*)(out + 500) = v;
  } else if (n == 1) {
    vbool4_t v = *(vbool4_t*)(in + 600);
    *(vbool4_t*)(out + 600) = v;
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vbool4_t v = *(vbool4_t*)(in + 900 + i);
    *(vbool4_t*)(out + 900 + i) = v;
  }
}

void f6 (int8_t * restrict in, int8_t * restrict out, int n)
{
  if (n == 0) {
    vbool2_t v = *(vbool2_t*)(in + 500);
    *(vbool2_t*)(out + 500) = v;
  } else if (n == 1) {
    vbool2_t v = *(vbool2_t*)(in + 600);
    *(vbool2_t*)(out + 600) = v;
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vbool2_t v = *(vbool2_t*)(in + 900 + i);
    *(vbool2_t*)(out + 900 + i) = v;
  }
}

void f7 (int8_t * restrict in, int8_t * restrict out, int n)
{
  if (n == 0) {
    vbool1_t v = *(vbool1_t*)(in + 500);
    *(vbool1_t*)(out + 500) = v;
  } else if (n == 1) {
    vbool1_t v = *(vbool1_t*)(in + 600);
    *(vbool1_t*)(out + 600) = v;
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vbool1_t v = *(vbool1_t*)(in + 900 + i);
    *(vbool1_t*)(out + 900 + i) = v;
  }
}

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf8,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf4,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf2,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m1,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m2,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m4,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m8,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf8,\s*t[au],\s*m[au]\s+\.L[0-9]+\:} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf4,\s*t[au],\s*m[au]\s+\.L[0-9]+\:} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf2,\s*t[au],\s*m[au]\s+\.L[0-9]+\:} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m1,\s*t[au],\s*m[au]\s+\.L[0-9]+\:} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m2,\s*t[au],\s*m[au]\s+\.L[0-9]+\:} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m4,\s*t[au],\s*m[au]\s+\.L[0-9]+\:} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m8,\s*t[au],\s*m[au]\s+\.L[0-9]+\:} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
