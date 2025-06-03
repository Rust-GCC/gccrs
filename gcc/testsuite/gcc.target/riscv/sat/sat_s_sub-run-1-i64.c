/* { dg-do run { target { riscv_v } } } */
/* { dg-additional-options "-std=c99" } */

#include "sat_arith.h"
#include "sat_arith_data.h"

#define T1 int64_t
#define T2 uint64_t

DEF_SAT_S_SUB_FMT_1_WRAP(T1, T2, INT64_MIN, INT64_MAX)

#define DATA             TEST_BINARY_DATA_WRAP(T1, sssub)
#define T                TEST_BINARY_STRUCT_DECL(T1, sssub)
#define RUN_BINARY(x, y) RUN_SAT_S_SUB_FMT_1_WRAP(T1, x, y)

#include "scalar_sat_binary_run_xxx.h"
