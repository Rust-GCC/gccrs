/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8 -fdump-tree-optimized" } */

#include "def.h"

DEF_CONVERT (fwcvt, _Float16, float, 4)
DEF_CONVERT (fwcvt, _Float16, float, 16)
DEF_CONVERT (fwcvt, _Float16, float, 32)
DEF_CONVERT (fwcvt, _Float16, float, 64)
DEF_CONVERT (fwcvt, _Float16, float, 128)
DEF_CONVERT (fwcvt, _Float16, float, 256)
DEF_CONVERT (fwcvt, _Float16, float, 512)
DEF_CONVERT (fwcvt, _Float16, float, 1024)

DEF_CONVERT (fwcvt, float, double, 4)
DEF_CONVERT (fwcvt, float, double, 16)
DEF_CONVERT (fwcvt, float, double, 32)
DEF_CONVERT (fwcvt, float, double, 64)
DEF_CONVERT (fwcvt, float, double, 128)
DEF_CONVERT (fwcvt, float, double, 256)

/* { dg-final { scan-assembler-times {vfwcvt\.f\.f\.v} 14 } } */
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
