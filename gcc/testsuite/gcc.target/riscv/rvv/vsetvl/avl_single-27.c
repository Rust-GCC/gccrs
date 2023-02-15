/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -fno-schedule-insns -fno-schedule-insns2 -fno-tree-vectorize" } */

#include "riscv_vector.h"

void f2 (int8_t * restrict in, int8_t * restrict out, int n, int m, unsigned cond, size_t vl)
{
  vbool64_t mask = *(vbool64_t*) (in + 1000000);

  vl = 101;
  if (cond > 0) {
    for (size_t i = 0; i < n; i++)
      {
        vint8mf8_t v = __riscv_vle8_v_i8mf8 (in + i, vl);
        __riscv_vse8_v_i8mf8 (out + i, v, vl);
      }
  } else {
    for (size_t i = 0; i < n; i++)
      {
        vint8mf8_t v = __riscv_vle8_v_i8mf8 (in + i + 1000, vl);
        __riscv_vse8_v_i8mf8 (out + i + 1000, v, vl);
      }
  }
    
  for (size_t i = 0; i < n; i++)
    {
      vfloat32mf2_t v = __riscv_vle32_v_f32mf2 ((float *)(in + i + 200), vl);
      __riscv_vse32_v_f32mf2 ((float *)(out + i + 200), v, vl);
      
      vfloat32mf2_t v2 = __riscv_vle32_v_f32mf2_tumu (mask, v, (float *)(in + i + 300), vl);
      __riscv_vse32_v_f32mf2_m (mask, (float *)(out + i + 300), v2, vl);
    }
}

/* { dg-final { scan-assembler-times {vsetvli\s+zero,\s*[a-x0-9]+,\s*e32,\s*mf2,\s*tu,\s*mu} 2 { target { no-opts "-O0" no-opts "-Os" no-opts "-g" no-opts "-funroll-loops" } } } } */
/* { dg-final { scan-assembler-times {vsetvli} 3 { target { no-opts "-O0" no-opts "-Os" no-opts "-g" no-opts "-funroll-loops" } } } } */
