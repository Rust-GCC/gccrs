/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -fno-schedule-insns -fno-schedule-insns2" } */

#include "riscv_vector.h"

void f (int8_t *base, int8_t *out, size_t m, size_t n) {
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j += 1) {
      vint8mf8_t v0 = __riscv_vle8_v_i8mf8(base + i, j);
      v0 = __riscv_vle8_v_i8mf8_tu(v0, base + i + 100, j);
      __riscv_vse8_v_i8mf8(out + i, v0, j);
    }
  }
}

/* { dg-final { scan-assembler-times {\.L[0-9]+\:\s+vsetvli\s+zero,\s*[a-x0-9]+,\s*e8,\s*mf8,\s*tu,\s*m[au]\s+vle8\.v\s+v[0-9]+,\s*0\s*\([a-x0-9]+\)} 1 { target { no-opts "-O0" no-opts "-O1" no-opts "-g" no-opts "-funroll-loops" } } } } */
/* { dg-final { scan-assembler-times {vsetvli} 1 { target { no-opts "-O0" no-opts "-g" no-opts "-funroll-loops" } } } } */
