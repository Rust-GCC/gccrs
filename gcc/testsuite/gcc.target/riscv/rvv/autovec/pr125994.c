/* { dg-compile } */
/* { dg-options "-march=rv64gc -mabi=lp64d -O3 -fdump-tree-vect-details" } */

#include <stdint.h>

void
AddWithIv (uint8_t *buf, uint8_t *Iv, uint64_t size)
{
  uint64_t i;
  for (i = 0; i < size; ++i)
    {
      buf[i] += Iv[i];
    }
}

/* { dg-final { scan-tree-dump "Both peeling and versioning will be applied." "vect" } } */
