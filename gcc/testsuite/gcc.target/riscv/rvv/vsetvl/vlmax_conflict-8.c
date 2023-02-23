/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -fno-tree-vectorize" } */

#include "riscv_vector.h"

void f (int32_t * restrict in, int32_t * restrict out, size_t n, size_t cond, size_t cond2)
{
  for (size_t i = 0; i < n; i++)
    {
      if (i != cond) {
        vbool1_t v = *(vbool1_t*)(in + i + 400);
        *(vbool1_t*)(out + i + 400) = v;
      } else if (i == cond2) {
        vuint16mf2_t v = *(vuint16mf2_t*)(in + i + 200);
        *(vuint16mf2_t*)(out + i + 200) = v;
      } else {
        vint8mf8_t v = *(vint8mf8_t*)(in + i + 100);
        *(vint8mf8_t*)(out + i + 100) = v;
      }
    }
}

/* { dg-final { scan-assembler-times {vsetvli} 5 { target { no-opts "-O0"  no-opts "-O1"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {j\s+\.L[0-9]+\s+\.L[0-9]+:\s+vlm\.v} 1 { target { no-opts "-O0" no-opts "-O1"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*m8,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0" no-opts "-O1"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf8,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" no-opts "-O1"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*mf2,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" no-opts "-O1"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
