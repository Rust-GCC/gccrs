/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d -fdump-tree-optimized" } */

#include "vec_sat_arith.h"

DEF_VEC_SAT_S_TRUNC_FMT_8(int16_t, int32_t, INT16_MIN, INT16_MAX)

/* { dg-final { scan-tree-dump-times ".SAT_TRUNC " 1 "optimized" { target { any-opts
     "-O3"
   } } } } */
/* { dg-final { scan-tree-dump-times ".SAT_TRUNC " 2 "optimized" { target { any-opts
     "-O2"
   } } } } */
/* { dg-final { scan-assembler-times {vnclip\.wi} 1 } } */
