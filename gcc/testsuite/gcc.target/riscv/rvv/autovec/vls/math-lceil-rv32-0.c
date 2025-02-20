/* { dg-do compile } */
/* { dg-options "-march=rv32gcv_zvl4096b -mabi=ilp32d -O3 -mrvv-max-lmul=m8 -ffast-math -fdump-tree-optimized" } */

#include "def.h"

DEF_OP_V_CVT (lceil, 1, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 2, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 4, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 8, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 16, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 32, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 64, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 128, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 256, double, int, __builtin_lceil)
DEF_OP_V_CVT (lceil, 512, double, int, __builtin_lceil)

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
/* { dg-final { scan-assembler-times {vfncvt\.x\.f\.w\s+v[0-9]+,\s*v[0-9]+} 9 } } */
