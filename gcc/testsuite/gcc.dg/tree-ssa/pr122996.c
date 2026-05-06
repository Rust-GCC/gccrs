/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

u64 f1 (u64 x) { return x == -1UL ? 0 : x + 1; }

u64 f2 (u64 x) { return x != -100UL ? x + 100 : 0; }

u64 f3 (u64 x) { return x == 1 ? 0 : x - 1; }

u32 f4 (u32 x) { return x == -5 ? 0 : x + 5; }

u16 f5 (u16 x) { return x == -30 ? 0 : x + 30; }

u8  f6 (u8 x)  { return x == 255 ? 0: x + 1; }

u64 g1 (u64 x) { return x == 10 ? 0 : x + 1; }

/* { dg-final { scan-tree-dump-times "if " 1 "optimized" } } */
