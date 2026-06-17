/* { dg-do run { target aarch64_sme_hw } } */
/* { dg-do compile { target { ! { aarch64_sme_hw } } } } */
/* { dg-options "-fbuilding-libgcc" } */

#pragma GCC target "+sme"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <arm_sme.h>

void
test (void) __arm_streaming
{
  unsigned char buf[16];

  memset (buf, 0x11, sizeof buf);

  assert (__arm_sc_memset (buf + 3, 0xaa, 5) == buf + 3);

  assert (buf[2] == 0x11);
  assert (buf[3] == 0xaa);
  assert (buf[7] == 0xaa);
  assert (buf[8] == 0x11);
}

int
main (void)
{
  test ();
  return 0;
}
