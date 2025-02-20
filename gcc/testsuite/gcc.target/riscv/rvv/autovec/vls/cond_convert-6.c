/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8 -ffast-math -fdump-tree-optimized" } */

#include "def.h"

DEF_COND_FP_CONVERT (fwcvt, qi, sf, float, 4)
DEF_COND_FP_CONVERT (fwcvt, qi, sf, float, 16)
DEF_COND_FP_CONVERT (fwcvt, qi, sf, float, 32)
DEF_COND_FP_CONVERT (fwcvt, qi, sf, float, 64)
DEF_COND_FP_CONVERT (fwcvt, qi, sf, float, 128)
DEF_COND_FP_CONVERT (fwcvt, qi, sf, float, 256)
DEF_COND_FP_CONVERT (fwcvt, qi, sf, float, 512)
DEF_COND_FP_CONVERT (fwcvt, qi, sf, float, 1024)

DEF_COND_FP_CONVERT (fwcvt, uqi, sf, float, 4)
DEF_COND_FP_CONVERT (fwcvt, uqi, sf, float, 16)
DEF_COND_FP_CONVERT (fwcvt, uqi, sf, float, 32)
DEF_COND_FP_CONVERT (fwcvt, uqi, sf, float, 64)
DEF_COND_FP_CONVERT (fwcvt, uqi, sf, float, 128)
DEF_COND_FP_CONVERT (fwcvt, uqi, sf, float, 256)
DEF_COND_FP_CONVERT (fwcvt, uqi, sf, float, 512)
DEF_COND_FP_CONVERT (fwcvt, uqi, sf, float, 1024)

DEF_COND_FP_CONVERT (fwcvt, hi, df, double, 4)
DEF_COND_FP_CONVERT (fwcvt, hi, df, double, 16)
DEF_COND_FP_CONVERT (fwcvt, hi, df, double, 32)
DEF_COND_FP_CONVERT (fwcvt, hi, df, double, 64)
DEF_COND_FP_CONVERT (fwcvt, hi, df, double, 128)
DEF_COND_FP_CONVERT (fwcvt, hi, df, double, 256)
DEF_COND_FP_CONVERT (fwcvt, hi, df, double, 512)

DEF_COND_FP_CONVERT (fwcvt, uhi, df, double, 4)
DEF_COND_FP_CONVERT (fwcvt, uhi, df, double, 16)
DEF_COND_FP_CONVERT (fwcvt, uhi, df, double, 32)
DEF_COND_FP_CONVERT (fwcvt, uhi, df, double, 64)
DEF_COND_FP_CONVERT (fwcvt, uhi, df, double, 128)
DEF_COND_FP_CONVERT (fwcvt, uhi, df, double, 256)
DEF_COND_FP_CONVERT (fwcvt, uhi, df, double, 512)

/* { dg-final { scan-assembler-times {vfwcvt\.f\.xu?\.v\s+v[0-9]+,\s*v[0-9]+,\s*v0.t} 30 } } */
/* { dg-final { scan-assembler-not {csrr} } } */
/* { dg-final { scan-assembler-not {vmerge} } } */
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
