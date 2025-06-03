/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8 -ffast-math -fdump-tree-optimized" } */

#include "def.h"

DEF_COND_CALL (cond_copysign, 4, v4hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 8, v8hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 16, v16hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 32, v32hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 64, v64hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 128, v128hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 256, v256hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 512, v512hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 1024, v1024hf, __builtin_copysignf16)
DEF_COND_CALL (cond_copysign, 2048, v2048hf, __builtin_copysignf16)

DEF_COND_CALL (cond_copysign, 4, v4sf, __builtin_copysignf)
DEF_COND_CALL (cond_copysign, 8, v8sf, __builtin_copysignf)
DEF_COND_CALL (cond_copysign, 16, v16sf, __builtin_copysignf)
DEF_COND_CALL (cond_copysign, 32, v32sf, __builtin_copysignf)
DEF_COND_CALL (cond_copysign, 64, v64sf, __builtin_copysignf)
DEF_COND_CALL (cond_copysign, 128, v128sf, __builtin_copysignf)
DEF_COND_CALL (cond_copysign, 256, v256sf, __builtin_copysignf)
DEF_COND_CALL (cond_copysign, 512, v512sf, __builtin_copysignf)
DEF_COND_CALL (cond_copysign, 1024, v1024sf, __builtin_copysignf)

DEF_COND_CALL (cond_copysign, 4, v4df, __builtin_copysign)
DEF_COND_CALL (cond_copysign, 8, v8df, __builtin_copysign)
DEF_COND_CALL (cond_copysign, 16, v16df, __builtin_copysign)
DEF_COND_CALL (cond_copysign, 32, v32df, __builtin_copysign)
DEF_COND_CALL (cond_copysign, 64, v64df, __builtin_copysign)
DEF_COND_CALL (cond_copysign, 128, v128df, __builtin_copysign)
DEF_COND_CALL (cond_copysign, 256, v256df, __builtin_copysign)
DEF_COND_CALL (cond_copysign, 512, v512df, __builtin_copysign)

/* { dg-final { scan-assembler-times {vfsgnj\.vv\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+,\s*v0.t} 27 } } */
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
