/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32" } */

#include "riscv_vector.h"

void f1 (void * restrict in, void * restrict out)
{
  vint8mf8_t v1 = *(vint8mf8_t*)(in + 1);
  vint16mf4_t v2;
  *(vint8mf8_t*)(out + 1) = v1;
  *(vint16mf4_t*)(out + 2) = v2;
}

void f2 (void * restrict in, void * restrict out)
{
  vint8mf8_t v1 = *(vint8mf8_t*)(in + 1);
  vint32mf2_t v2;
  *(vint8mf8_t*)(out + 1) = v1;
  *(vint32mf2_t*)(out + 2) = v2;
}

void f3 (void * restrict in, void * restrict out)
{
  vint8mf8_t v1 = *(vint8mf8_t*)(in + 1);
  vint64m1_t v2;
  *(vint8mf8_t*)(out + 1) = v1;
  *(vint64m1_t*)(out + 2) = v2;
}

void f4 (void * restrict in, void * restrict out)
{
  vint8mf4_t v1 = *(vint8mf4_t*)(in + 1);
  vint16mf2_t v2;
  *(vint8mf4_t*)(out + 1) = v1;
  *(vint16mf2_t*)(out + 2) = v2;
}

void f5 (void * restrict in, void * restrict out)
{
  vint8mf4_t v1 = *(vint8mf4_t*)(in + 1);
  vint32m1_t v2;
  *(vint8mf4_t*)(out + 1) = v1;
  *(vint32m1_t*)(out + 2) = v2;
}

void f6 (void * restrict in, void * restrict out)
{
  vint8mf4_t v1 = *(vint8mf4_t*)(in + 1);
  vint64m2_t v2;
  *(vint8mf4_t*)(out + 1) = v1;
  *(vint64m2_t*)(out + 2) = v2;
}

void f7 (void * restrict in, void * restrict out)
{
  vint8mf2_t v1 = *(vint8mf2_t*)(in + 1);
  vint16m1_t v2;
  *(vint8mf2_t*)(out + 1) = v1;
  *(vint16m1_t*)(out + 2) = v2;
}

void f8 (void * restrict in, void * restrict out)
{
  vint8mf2_t v1 = *(vint8mf2_t*)(in + 1);
  vint32m2_t v2;
  *(vint8mf2_t*)(out + 1) = v1;
  *(vint32m2_t*)(out + 2) = v2;
}

void f9 (void * restrict in, void * restrict out)
{
  vint8mf2_t v1 = *(vint8mf2_t*)(in + 1);
  vint64m4_t v2;
  *(vint8mf2_t*)(out + 1) = v1;
  *(vint64m4_t*)(out + 2) = v2;
}

void f10 (void * restrict in, void * restrict out)
{
  vint8m1_t v1 = *(vint8m1_t*)(in + 1);
  vint16m2_t v2;
  *(vint8m1_t*)(out + 1) = v1;
  *(vint16m2_t*)(out + 2) = v2;
}

void f11 (void * restrict in, void * restrict out)
{
  vint8m1_t v1 = *(vint8m1_t*)(in + 1);
  vint32m4_t v2;
  *(vint8m1_t*)(out + 1) = v1;
  *(vint32m4_t*)(out + 2) = v2;
}

void f12 (void * restrict in, void * restrict out)
{
  vint8m1_t v1 = *(vint8m1_t*)(in + 1);
  vint64m8_t v2;
  *(vint8m1_t*)(out + 1) = v1;
  *(vint64m8_t*)(out + 2) = v2;
}

void f13 (void * restrict in, void * restrict out)
{
  vint8m2_t v1 = *(vint8m2_t*)(in + 1);
  vint16m4_t v2;
  *(vint8m2_t*)(out + 1) = v1;
  *(vint16m4_t*)(out + 2) = v2;
}

void f14 (void * restrict in, void * restrict out)
{
  vint8m2_t v1 = *(vint8m2_t*)(in + 1);
  vint32m8_t v2;
  *(vint8m2_t*)(out + 1) = v1;
  *(vint32m8_t*)(out + 2) = v2;
}

void f15 (void * restrict in, void * restrict out)
{
  vint8m4_t v1 = *(vint8m4_t*)(in + 1);
  vint16m8_t v2;
  *(vint8m4_t*)(out + 1) = v1;
  *(vint16m8_t*)(out + 2) = v2;
}

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*mf4,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e32,\s*mf2,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e64,\s*m1,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*mf2,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e32,\s*m1,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e64,\s*m2,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*m1,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e32,\s*m2,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e64,\s*m4,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*m2,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e32,\s*m4,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e64,\s*m8,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*m4,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e32,\s*m8,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*m8,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" } } } } */

/* { dg-final { scan-assembler-times {vsetvli} 15 { target { no-opts "-O0" } } } } */
