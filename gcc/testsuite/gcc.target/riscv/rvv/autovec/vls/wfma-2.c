/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8 -fdump-tree-optimized" } */

#include "def.h"

DEF_FMA_WVV(wfma, 4, _Float16, float)
DEF_FMA_WVV(wfma, 8, _Float16, float)
DEF_FMA_WVV(wfma, 16, _Float16, float)
DEF_FMA_WVV(wfma, 32, _Float16, float)
DEF_FMA_WVV(wfma, 64, _Float16, float)
DEF_FMA_WVV(wfma, 128, _Float16, float)
DEF_FMA_WVV(wfma, 256, _Float16, float)
DEF_FMA_WVV(wfma, 512, _Float16, float)
DEF_FMA_WVV(wfma, 1024, _Float16, float)

DEF_FMA_WVV(wfma, 4, float, double)
DEF_FMA_WVV(wfma, 8, float, double)
DEF_FMA_WVV(wfma, 16, float, double)
DEF_FMA_WVV(wfma, 32, float, double)
DEF_FMA_WVV(wfma, 64, float, double)
DEF_FMA_WVV(wfma, 128, float, double)
DEF_FMA_WVV(wfma, 256, float, double)
DEF_FMA_WVV(wfma, 512, float, double)

/* { dg-final { scan-assembler-times {vfwmacc\.vv} 17 } } */
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
