/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -fno-schedule-insns -fno-schedule-insns2 -mrvv-max-lmul=m8" } */

#include "def.h"

DEF_OP_VI_15 (and, 1, int8_t, &)
DEF_OP_VI_15 (and, 2, int8_t, &)
DEF_OP_VI_15 (and, 4, int8_t, &)
DEF_OP_VI_15 (and, 8, int8_t, &)
DEF_OP_VI_15 (and, 16, int8_t, &)
DEF_OP_VI_15 (and, 32, int8_t, &)
DEF_OP_VI_15 (and, 64, int8_t, &)
DEF_OP_VI_15 (and, 128, int8_t, &)
DEF_OP_VI_15 (and, 256, int8_t, &)
DEF_OP_VI_15 (and, 512, int8_t, &)
DEF_OP_VI_15 (and, 1024, int8_t, &)
DEF_OP_VI_15 (and, 2048, int8_t, &)
DEF_OP_VI_15 (and, 4096, int8_t, &)

DEF_OP_VI_15 (and, 1, int16_t, &)
DEF_OP_VI_15 (and, 2, int16_t, &)
DEF_OP_VI_15 (and, 4, int16_t, &)
DEF_OP_VI_15 (and, 8, int16_t, &)
DEF_OP_VI_15 (and, 16, int16_t, &)
DEF_OP_VI_15 (and, 32, int16_t, &)
DEF_OP_VI_15 (and, 64, int16_t, &)
DEF_OP_VI_15 (and, 128, int16_t, &)
DEF_OP_VI_15 (and, 256, int16_t, &)
DEF_OP_VI_15 (and, 512, int16_t, &)
DEF_OP_VI_15 (and, 1024, int16_t, &)
DEF_OP_VI_15 (and, 2048, int16_t, &)

DEF_OP_VI_15 (and, 1, int32_t, &)
DEF_OP_VI_15 (and, 2, int32_t, &)
DEF_OP_VI_15 (and, 4, int32_t, &)
DEF_OP_VI_15 (and, 8, int32_t, &)
DEF_OP_VI_15 (and, 16, int32_t, &)
DEF_OP_VI_15 (and, 32, int32_t, &)
DEF_OP_VI_15 (and, 64, int32_t, &)
DEF_OP_VI_15 (and, 128, int32_t, &)
DEF_OP_VI_15 (and, 256, int32_t, &)
DEF_OP_VI_15 (and, 512, int32_t, &)
DEF_OP_VI_15 (and, 1024, int32_t, &)

DEF_OP_VI_15 (and, 1, int64_t, &)
DEF_OP_VI_15 (and, 2, int64_t, &)
DEF_OP_VI_15 (and, 4, int64_t, &)
DEF_OP_VI_15 (and, 8, int64_t, &)
DEF_OP_VI_15 (and, 16, int64_t, &)
DEF_OP_VI_15 (and, 32, int64_t, &)
DEF_OP_VI_15 (and, 64, int64_t, &)
DEF_OP_VI_15 (and, 128, int64_t, &)
DEF_OP_VI_15 (and, 256, int64_t, &)
DEF_OP_VI_15 (and, 512, int64_t, &)

/* { dg-final { scan-assembler-times {vand\.vi\s+v[0-9]+,\s*v[0-9]+,\s*15} 42 } } */
/* { dg-final { scan-assembler-not {csrr} } } */
