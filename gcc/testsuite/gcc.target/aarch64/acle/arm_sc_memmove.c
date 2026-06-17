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
  char dst[16] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};

  assert (__arm_sc_memmove (dst + 4, src, 0) == dst + 4);
  assert (memcmp (dst, "xxxxxxxx", 8) == 0);

  assert (__arm_sc_memmove (dst + 1, src + 2, 3) == dst + 1);
  assert (memcmp (dst, "xcdexxxx", 8) == 0);

  __arm_sc_memcpy (dst, src, sizeof src);
  assert (__arm_sc_memmove (dst + 2, dst, 4) == dst + 2);
  assert (memcmp (dst, "ababcd", 6) == 0);

  __arm_sc_memcpy (dst, src, sizeof src);
  assert (__arm_sc_memmove (dst, dst + 2, 4) == dst);
  assert (memcmp (dst, "cdefef", 6) == 0);

  __arm_sc_memcpy (dst, src, sizeof src);
  assert (__arm_sc_memmove (dst + 1, dst + 1, 3) == dst + 1);
  assert (memcmp (dst, "abcdef", 6) == 0);

  __arm_sc_memcpy (dst, src, sizeof src);
  assert (__arm_sc_memmove (dst + 3, dst, 0) == dst + 3);
  assert (memcmp (dst, "abcdef", 6) == 0);

  return 0;
}
