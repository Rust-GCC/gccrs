/* { dg-do run } */
/* { dg-require-effective-target aarch64_variant_pcs } */
/* { dg-options "-O1 -march=armv8-a" } */

#include <assert.h>
#include <string.h>

#include <arm_sme.h>

int
main (void)
{
  char src[] = "abcdef";
  char dst[8] = { 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' };

  assert (__arm_sc_memcpy (dst + 4, src, 0) == dst + 4);
  assert (memcmp (dst, "xxxxxxxx", 8) == 0);

  assert (__arm_sc_memcpy (dst + 1, src + 2, 3) == dst + 1);
  assert (memcmp (dst, "xcdexxxx", 8) == 0);

  return 0;
}
