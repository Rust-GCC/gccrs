/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

typedef unsigned int u32;
typedef unsigned long u64;

u64 g1 (u64 x) { return x != 10 ? x + 20 : 40; }

u32 g2 (u32 x) { return x == 100 ? 10 : x + 166; }

/* { dg-final { scan-tree-dump-times "if " 2 "optimized" } } */
