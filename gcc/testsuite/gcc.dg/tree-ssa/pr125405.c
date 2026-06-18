/* PR tree-optimization/125405 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

typedef unsigned long xlen_t;

xlen_t
sext8_hd (xlen_t x)
{
  return (x & 0x7fUL) - (x & 0x80UL);
}

xlen_t
sext16_hd (xlen_t x)
{
  return (x & 0x7fffUL) - (x & 0x8000UL);
}

/* { dg-final { scan-tree-dump-times "\\(signed char\\)" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-times "\\(signed short\\)" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-not " & 127" "optimized" } } */
/* { dg-final { scan-tree-dump-not " & 128" "optimized" } } */
/* { dg-final { scan-tree-dump-not " & 32767" "optimized" } } */
/* { dg-final { scan-tree-dump-not " & 32768" "optimized" } } */