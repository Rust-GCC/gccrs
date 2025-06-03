/* Test that we do not have error when compile */
/* { dg-do compile } */
/* { dg-options "-march=rv64gc_zve64d -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vfloat64m1_t
__attribute__((target("arch=+v")))
test_1 (vfloat64m1_t a, vfloat64m1_t b, size_t vl)
{
  return __riscv_vfadd_vv_f64m1 (a, b, vl);
}
