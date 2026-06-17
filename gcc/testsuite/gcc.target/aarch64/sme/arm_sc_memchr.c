/* { dg-do run { target aarch64_sme_hw } } */
/* { dg-do compile { target { ! { aarch64_sme_hw } } } } */
/* { dg-options "-O2 -fbuilding-libgcc" } */

#pragma GCC target "+sme"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <arm_sme.h>

unsigned char basic[] = {'a', 'b', 'c', 'd', 'e', 'f'};

void
test (void) __arm_streaming
{
  assert (__arm_sc_memchr (basic, 'x', sizeof basic) == NULL);
  assert (__arm_sc_memchr (basic, 'd', 4) == basic + 3);
}

int
main (void)
{
  test ();
  return 0;
}
