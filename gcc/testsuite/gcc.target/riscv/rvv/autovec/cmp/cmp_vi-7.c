/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d -O3 -mrvv-max-lmul=dynamic -fdump-tree-optimized-details" } */

#include "macro.h"

CMP_VI (eq_char, char, 4, ==, 15)
CMP_VI (eq_short, short, 4, ==, 15)
CMP_VI (eq_int, int, 4, ==, 15)
CMP_VI (eq_long, long, 4, ==, 15)
CMP_VI (eq_unsigned_char, unsigned char, 4, ==, 15)
CMP_VI (eq_unsigned_short, unsigned short, 4, ==, 15)
CMP_VI (eq_unsigned_int, unsigned int, 4, ==, 15)
CMP_VI (eq_unsigned_long, unsigned long, 4, ==, 15)

/* { dg-final { scan-assembler-times {vmseq\.vi} 32 } } */
/* { dg-final { scan-assembler-not {vmseq\.vv} } } */
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
