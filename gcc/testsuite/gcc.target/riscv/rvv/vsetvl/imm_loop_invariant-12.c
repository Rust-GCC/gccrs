/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -fno-schedule-insns -fno-schedule-insns2" } */

#include "riscv_vector.h"

void f (void * restrict in, void * restrict out, int n)
{
  vint8mf8_t v = __riscv_vle8_v_i8mf8(in + 100, 4);
  vint8mf8_t v1 = __riscv_vle8_v_i8mf8(in + 1, 4);
  vint16mf4_t v2 = __riscv_vle16_v_i16mf4(in + 2, 4);
  vint32mf2_t v3 = __riscv_vle32_v_i32mf2(in + 3, 4);
  vfloat32mf2_t v4 = __riscv_vle32_v_f32mf2(in + 4, 4);
  
  __riscv_vse8_v_i8mf8 (out + 1, v1, 4);
  __riscv_vse16_v_i16mf4 (out + 2, v2, 4);
  __riscv_vse32_v_i32mf2 (out + 3, v3, 4);
  __riscv_vse32_v_f32mf2 (out + 4, v4, 4);
  
  for (int i = 0; i < n; i++)
    {
      v = __riscv_vle8_v_i8mf8_tu(v, in + i + 5, 4);
      __riscv_vse8_v_i8mf8 (out + i + 5, v, 4);
    }
}

/* { dg-final { scan-assembler-times {\.L[0-9]+\:\s+vle8\.v\s+v[0-9]+,\s*0\s*\([a-x0-9]+\)} 1 { target { no-opts "-O0" no-opts "-Os" no-opts "-g" no-opts "-funroll-loops" } } } } */
/* { dg-final { scan-assembler-times {vsetivli\s+zero,\s*4,\s*e8,\s*mf8,\s*tu,\s*m[au]} 1 { target { no-opts "-O0" no-opts "-g" no-opts "-funroll-loops" } } } } */
/* { dg-final { scan-assembler-times {vsetivli} 1 { target { no-opts "-O0" no-opts "-g" no-opts "-funroll-loops" } } } } */
