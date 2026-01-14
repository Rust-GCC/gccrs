/* { dg-do compile } */
/* { dg-additional-options "-O3 --param vect-epilogues-nomask=0" } */
#include <stdint-gcc.h>

uint64_t
x264_pixel_var_8x8 (uint8_t *pix, int i_stride)
{
  uint32_t sum = 0, sqr = 0;
  for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
	{
	  sum += pix[x];
	  sqr += pix[x] * pix[x];
	}
      pix += i_stride;
    }
  return sum + ((uint64_t)sqr << 32);
}

/* Verify that size-1 reductions fall back to single-lane reduction chains.  */
/* { dg-final { scan-tree-dump "Starting SLP discovery of reduction chain" "vect" } } */
/* { dg-final { scan-tree-dump-not "SLP discovery of reduction chain failed" "vect" } } */
/* { dg-final { scan-tree-dump-not "SLP discovery of size 2 reduction group" "vect" } } */
/* { dg-final { scan-tree-dump-not "SLP discovery of size 1 reduction group" "vect" } } */
