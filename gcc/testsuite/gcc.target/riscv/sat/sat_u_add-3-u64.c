/* { dg-do compile } */
/* { dg-options "-march=rv64gc -mabi=lp64d -fdump-tree-optimized -fno-schedule-insns -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "sat_arith.h"

/*
** sat_u_add_uint64_t_fmt_3:
** add\s+[atx][0-9]+,\s*a0,\s*a1
** sltu\s+[atx][0-9]+,\s*[atx][0-9]+,\s*[atx][0-9]+
** neg\s+[atx][0-9]+,\s*[atx][0-9]+
** or\s+a0,\s*[atx][0-9]+,\s*[atx][0-9]+
** ret
*/
DEF_SAT_U_ADD_FMT_3(uint64_t)

/* { dg-final { scan-tree-dump-times ".SAT_ADD " 1 "optimized" } } */
