/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

u64 f1 (u64 x) { return x != 5 ? x + 10 : 15; }

u32 f2 (u32 x) { return x != 20 ? x - 5 : 15; }

u16 f3 (u16 x) { return x == 100 ? 150 : x + 50; }

u8  f4 (u8 x)  { return x != 250 ? x + 2 : 252; }

u8  f5 (u8 x) {
  if (x == 100)
    return 10;
  else
    return x + 166;
}

/* { dg-final { scan-tree-dump-not "if " "optimized" } } */
