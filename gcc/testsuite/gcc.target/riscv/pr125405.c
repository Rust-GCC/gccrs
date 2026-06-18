/* PR tree-optimization/125405 */
/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc -mabi=lp64d" } */

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

/* { dg-final { scan-assembler-times "\\mslliw\\M" 2 } } */
/* { dg-final { scan-assembler-times "\\msraiw\\M" 2 } } */
/* { dg-final { scan-assembler-not "\\mandi\\M" } } */
/* { dg-final { scan-assembler-not "\\msub\\M" } } */