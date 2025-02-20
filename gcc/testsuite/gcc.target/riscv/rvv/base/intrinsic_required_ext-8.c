/* { dg-do compile } */
/* { dg-options "-march=rv64gc -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vuint64m1_t
__attribute__((target("arch=+v")))
test_1 (vuint64m1_t dest, vuint64m1_t op_1, vuint64m1_t op_2, size_t vl)
{
  return __riscv_vsha2ms_vv_u64m1 (dest, op_1, op_2, vl); /* { dg-error {built-in function '__riscv_vsha2ms_vv_u64m1\(dest,  op_1,  op_2,  vl\)' requires the 'zvknhb' ISA extension} } */
}
