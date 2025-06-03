/* Test that we do not have error when compile */
/* { dg-do compile } */
/* { dg-options "-march=rv64gc -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vuint32m1_t
__attribute__((target("arch=+zvkned")))
test_1 (vuint32m1_t a, vuint32m1_t b, size_t vl)
{
  return __riscv_vaesdm_vv_u32m1 (a, b, vl);
}
