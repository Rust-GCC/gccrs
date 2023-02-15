/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -fno-schedule-insns -fno-schedule-insns2" } */

#include "riscv_vector.h"

void f (int8_t * restrict in, int8_t * restrict out, int n, int m, int cond)
{
  size_t vl = 101;
  vbool64_t mask = *(vbool64_t*) (in + 1000000);
  for (size_t j = 0; j < m; j++){
    for (size_t i = 0; i < n; i++)
      {
        vint8mf8_t v = __riscv_vle8_v_i8mf8 (in + i + j, vl);
        __riscv_vse8_v_i8mf8 (out + i, v, vl);
        
        vint8mf8_t v2 = __riscv_vle8_v_i8mf8_tumu (mask, v, in + i + j + 100, vl);
        __riscv_vse8_v_i8mf8_m (mask, out + i + j + 100, v2, vl);
      }
      
    for (size_t i = 0; i < n; i++)
      {
        vfloat32mf2_t v = __riscv_vle32_v_f32mf2 ((float *)(in + i + j + 200), vl);
        __riscv_vse32_v_f32mf2 ((float *)(out + i + j + 200), v, vl);
      }
  }
}

/* { dg-final { scan-assembler-times {vsetvli\s+zero,\s*[a-x0-9]+,\s*e8,\s*mf8,\s*tu,\s*mu} 1 { target { no-opts "-O0" no-opts "-g" no-opts "-funroll-loops" } } } } */
/* { dg-final { scan-assembler-times {vsetvli} 2 { target { no-opts "-O0" no-opts "-g" no-opts "-funroll-loops" } } } } */
