/* { dg-do assemble } */
/* { dg-options "-march=rv32gc_xtheadvector -mabi=ilp32d -O2 -save-temps" { target { rv32 } } } */
/* { dg-options "-march=rv64gc_xtheadvector -mabi=lp64d -O2 -save-temps" { target { rv64 } } } */

#include "riscv_vector.h"

/* Test that static rounding modes do not cause string splicing in th_asm_output_opcode. */

vint32m1_t test_vfncvt_ftoi(vfloat64m2_t src, size_t vl) {
  return __riscv_vfncvt_rtz_x_f_w_i32m1(src, vl);
}

vfloat32m1_t test_vfncvt_ftof(vfloat64m2_t src, size_t vl) {
  return __riscv_vfncvt_rod_f_f_w_f32m1(src, vl);
}

vint64m4_t test_vfwcvt_ftoi(vfloat32m2_t src, size_t vl) {
  return __riscv_vfwcvt_rtz_x_f_v_i64m4(src, vl);
}

/* { dg-final { scan-assembler-not "\\.rtz" } } */
/* { dg-final { scan-assembler-not "\\.rod" } } */
/* { dg-final { scan-assembler-not "th\\.vfncvt\\..*\\.w" } } */
/* { dg-final { scan-assembler-times "th\\.vfncvt\\.x\\.f\\.v\\s+" 1 } } */
/* { dg-final { scan-assembler-times "th\\.vfncvt\\.f\\.f\\.v\\s+" 1 } } */
/* { dg-final { scan-assembler-times "th\\.vfwcvt\\.x\\.f\\.v\\s+" 1 } } */
