/* { dg-do run { target aarch64_sme_hw } } */
/* { dg-do compile { target { ! { aarch64_sme_hw } } } } */
/* { dg-options "-O2 -fbuilding-libgcc" } */

#pragma GCC target "+sme"

#include <assert.h>
#include <string.h>

#include <arm_sme.h>

void
test (void) __arm_streaming
{
  char src[] = "abcdef";
  char dst[8] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
  assert (__arm_sc_memcpy (dst + 1, src + 2, 3) == dst + 1);
  assert (memcmp (dst, "xcdexxxx", 8) == 0);
}

int
main (void)
{
  test ();
  return 0;
}
