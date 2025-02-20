/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8 -fdump-tree-optimized" } */

#include "def.h"

DEF_FNMA_VV (fnma, 2, int8_t)
DEF_FNMA_VV (fnma, 4, int8_t)
DEF_FNMA_VV (fnma, 8, int8_t)
DEF_FNMA_VV (fnma, 16, int8_t)
DEF_FNMA_VV (fnma, 32, int8_t)
DEF_FNMA_VV (fnma, 64, int8_t)
DEF_FNMA_VV (fnma, 128, int8_t)
DEF_FNMA_VV (fnma, 256, int8_t)
DEF_FNMA_VV (fnma, 512, int8_t)
DEF_FNMA_VV (fnma, 1024, int8_t)
DEF_FNMA_VV (fnma, 2048, int8_t)
DEF_FNMA_VV (fnma, 4096, int8_t)

DEF_FNMA_VV (fnma, 2, uint8_t)
DEF_FNMA_VV (fnma, 4, uint8_t)
DEF_FNMA_VV (fnma, 8, uint8_t)
DEF_FNMA_VV (fnma, 16, uint8_t)
DEF_FNMA_VV (fnma, 32, uint8_t)
DEF_FNMA_VV (fnma, 64, uint8_t)
DEF_FNMA_VV (fnma, 128, uint8_t)
DEF_FNMA_VV (fnma, 256, uint8_t)
DEF_FNMA_VV (fnma, 512, uint8_t)
DEF_FNMA_VV (fnma, 1024, uint8_t)
DEF_FNMA_VV (fnma, 2048, uint8_t)
DEF_FNMA_VV (fnma, 4096, uint8_t)

/* { dg-final { scan-assembler-times {vnms[a-u][b-c]\.vv} 24 } } */
/* { dg-final { scan-assembler-not {csrr} } } */
/* { dg-final { scan-tree-dump-not "1,1" "optimized" } } */
/* { dg-final { scan-tree-dump-not "2,2" "optimized" } } */
/* { dg-final { scan-tree-dump-not "4,4" "optimized" } } */
/* { dg-final { scan-tree-dump-not "16,16" "optimized" } } */
/* { dg-final { scan-tree-dump-not "32,32" "optimized" } } */
/* { dg-final { scan-tree-dump-not "64,64" "optimized" } } */
/* { dg-final { scan-tree-dump-not "128,128" "optimized" } } */
/* { dg-final { scan-tree-dump-not "256,256" "optimized" } } */
/* { dg-final { scan-tree-dump-not "512,512" "optimized" } } */
/* { dg-final { scan-tree-dump-not "1024,1024" "optimized" } } */
/* { dg-final { scan-tree-dump-not "2048,2048" "optimized" } } */
/* { dg-final { scan-tree-dump-not "4096,4096" "optimized" } } */
