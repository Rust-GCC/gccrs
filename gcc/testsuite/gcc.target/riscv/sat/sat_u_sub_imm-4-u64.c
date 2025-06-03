/* { dg-do compile } */
/* { dg-options "-march=rv64gc -mabi=lp64d -fdump-tree-optimized -fno-schedule-insns -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "sat_arith.h"

/*
** sat_u_sub_imm82_uint64_t_fmt_2:
** addi\s+[atx][0-9]+,\s*[atx][0-9]+,\s*-82
** sltiu\s+a0,\s*[atx][0-9]+,\s*82
** addi\s+a0,\s*a0,\s*-1
** and\s+a0,\s*a0,\s*[atx][0-9]+
** ret
*/

DEF_SAT_U_SUB_IMM_FMT_2(uint64_t, 82)

/* { dg-final { scan-tree-dump-times ".SAT_SUB " 1 "optimized" } } */
