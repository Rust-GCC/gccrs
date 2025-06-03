/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8 -ffast-math -fdump-tree-optimized" } */

#include "def.h"

DEF_COND_FP_CONVERT (fwcvt, qi, hf, _Float16, 4)
DEF_COND_FP_CONVERT (fwcvt, qi, hf, _Float16, 16)
DEF_COND_FP_CONVERT (fwcvt, qi, hf, _Float16, 32)
DEF_COND_FP_CONVERT (fwcvt, qi, hf, _Float16, 64)
DEF_COND_FP_CONVERT (fwcvt, qi, hf, _Float16, 128)
DEF_COND_FP_CONVERT (fwcvt, qi, hf, _Float16, 256)
DEF_COND_FP_CONVERT (fwcvt, qi, hf, _Float16, 512)
DEF_COND_FP_CONVERT (fwcvt, qi, hf, _Float16, 1024)

DEF_COND_FP_CONVERT (fwcvt, uqi, hf, _Float16, 4)
DEF_COND_FP_CONVERT (fwcvt, uqi, hf, _Float16, 16)
DEF_COND_FP_CONVERT (fwcvt, uqi, hf, _Float16, 32)
DEF_COND_FP_CONVERT (fwcvt, uqi, hf, _Float16, 64)
DEF_COND_FP_CONVERT (fwcvt, uqi, hf, _Float16, 128)
DEF_COND_FP_CONVERT (fwcvt, uqi, hf, _Float16, 256)
DEF_COND_FP_CONVERT (fwcvt, uqi, hf, _Float16, 512)
DEF_COND_FP_CONVERT (fwcvt, uqi, hf, _Float16, 1024)

DEF_COND_FP_CONVERT (fwcvt, hi, sf, float, 4)
DEF_COND_FP_CONVERT (fwcvt, hi, sf, float, 16)
DEF_COND_FP_CONVERT (fwcvt, hi, sf, float, 32)
DEF_COND_FP_CONVERT (fwcvt, hi, sf, float, 64)
DEF_COND_FP_CONVERT (fwcvt, hi, sf, float, 128)
DEF_COND_FP_CONVERT (fwcvt, hi, sf, float, 256)
DEF_COND_FP_CONVERT (fwcvt, hi, sf, float, 512)
DEF_COND_FP_CONVERT (fwcvt, hi, sf, float, 1024)

DEF_COND_FP_CONVERT (fwcvt, uhi, sf, float, 4)
DEF_COND_FP_CONVERT (fwcvt, uhi, sf, float, 16)
DEF_COND_FP_CONVERT (fwcvt, uhi, sf, float, 32)
DEF_COND_FP_CONVERT (fwcvt, uhi, sf, float, 64)
DEF_COND_FP_CONVERT (fwcvt, uhi, sf, float, 128)
DEF_COND_FP_CONVERT (fwcvt, uhi, sf, float, 256)
DEF_COND_FP_CONVERT (fwcvt, uhi, sf, float, 512)
DEF_COND_FP_CONVERT (fwcvt, uhi, sf, float, 1024)

DEF_COND_FP_CONVERT (fwcvt, si, df, double, 4)
DEF_COND_FP_CONVERT (fwcvt, si, df, double, 16)
DEF_COND_FP_CONVERT (fwcvt, si, df, double, 32)
DEF_COND_FP_CONVERT (fwcvt, si, df, double, 64)
DEF_COND_FP_CONVERT (fwcvt, si, df, double, 128)
DEF_COND_FP_CONVERT (fwcvt, si, df, double, 256)
DEF_COND_FP_CONVERT (fwcvt, si, df, double, 512)

DEF_COND_FP_CONVERT (fwcvt, usi, df, double, 4)
DEF_COND_FP_CONVERT (fwcvt, usi, df, double, 16)
DEF_COND_FP_CONVERT (fwcvt, usi, df, double, 32)
DEF_COND_FP_CONVERT (fwcvt, usi, df, double, 64)
DEF_COND_FP_CONVERT (fwcvt, usi, df, double, 128)
DEF_COND_FP_CONVERT (fwcvt, usi, df, double, 256)
DEF_COND_FP_CONVERT (fwcvt, usi, df, double, 512)

/* { dg-final { scan-assembler-times {vfwcvt\.f\.xu?\.v\s+v[0-9]+,\s*v[0-9]+,\s*v0.t} 46 } } */
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
