/* PR tree-optimization/125405 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

unsigned _BitInt(32)
sext8_bitint32 (unsigned _BitInt(32) x)
{
  return (x & (unsigned _BitInt(32)) 0x7f)
         - (x & (unsigned _BitInt(32)) 0x80);
}

unsigned _BitInt(32)
sext16_bitint32 (unsigned _BitInt(32) x)
{
  return (x & (unsigned _BitInt(32)) 0x7fff)
         - (x & (unsigned _BitInt(32)) 0x8000);
}

unsigned _BitInt(32)
no_sext_bad_masks (unsigned _BitInt(32) x)
{
  return (x & (unsigned _BitInt(32)) 0x7e)
         - (x & (unsigned _BitInt(32)) 0x80);
}

unsigned _BitInt(32)
no_sext_bad_pair (unsigned _BitInt(32) x)
{
  return (x & (unsigned _BitInt(32)) 0xff)
         - (x & (unsigned _BitInt(32)) 0x80);
}

/* { dg-final { scan-tree-dump-times "\\(signed char\\)" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-times "\\(signed short\\)" 1 "optimized" } } */
/* { dg-final { scan-tree-dump " & 126" "optimized" } } */
/* { dg-final { scan-tree-dump " & 255" "optimized" } } */