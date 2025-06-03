/* { dg-do run { target { riscv_v } } } */
/* { dg-additional-options "-std=c99" } */

#include "vec_sat_arith.h"
#include "vec_sat_data.h"

#define T uint64_t
#define RUN(T, out, in, expect, IMM, N) \
  RUN_VEC_SAT_U_SUB_IMM_FMT_2_WRAP (T, out, in, expect, IMM, N)

DEF_VEC_SAT_U_SUB_IMM_FMT_2_WRAP (T,   0)
DEF_VEC_SAT_U_SUB_IMM_FMT_2_WRAP (T,   1)
DEF_VEC_SAT_U_SUB_IMM_FMT_2_WRAP (T, 18446744073709551614u)
DEF_VEC_SAT_U_SUB_IMM_FMT_2_WRAP (T, 18446744073709551615u)

int
main ()
{
  T out[N];
  T (*d)[2][N] = TEST_UNARY_DATA_WRAP (T, sat_u_sub_imm);

  RUN (T, out, d[4][0], d[4][1],   0, N);
  RUN (T, out, d[5][0], d[5][1],   1, N);
  RUN (T, out, d[6][0], d[6][1], 18446744073709551614u, N);
  RUN (T, out, d[7][0], d[7][1], 18446744073709551615u, N);

  return 0;
}
