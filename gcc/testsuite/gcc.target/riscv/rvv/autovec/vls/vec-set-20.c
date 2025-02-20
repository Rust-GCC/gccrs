/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -fno-schedule-insns -fno-schedule-insns2 -mrvv-max-lmul=m8" } */

#include "def.h"

DEF_VEC_SET_SCALAR_INDEX (vec_set, v1hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v2hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v4hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v8hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v16hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v32hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v64hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v128hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v256hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v512hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v1024hf, _Float16)
DEF_VEC_SET_SCALAR_INDEX (vec_set, v2048hf, _Float16)

/* { dg-final { scan-assembler-not {csrr} } } */
/* { dg-final { scan-assembler-times {vslideup\.vx\s+v[0-9]+,\s*v[0-9]+,\s*[atx][0-9]+} 11 } } */
