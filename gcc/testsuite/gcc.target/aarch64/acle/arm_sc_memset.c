/* { dg-do run } */
/* { dg-require-effective-target aarch64_variant_pcs } */
/* { dg-options "-O1 -march=armv8-a" } */

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <arm_sme.h>

int
main (void)
{
  unsigned char buf[16];

  memset (buf, 0x11, sizeof buf);

  assert (__arm_sc_memset (buf + 3, 0xaa, 5) == buf + 3);

  assert (buf[2] == 0x11);
  assert (buf[3] == 0xaa);
  assert (buf[7] == 0xaa);
  assert (buf[8] == 0x11);

  assert (__arm_sc_memset (buf + 4, 0x55, 0) == buf + 4);
  assert (buf[4] == 0xaa);

  assert (__arm_sc_memset (buf, 0x1234, 2) == buf);
  assert (buf[0] == 0x34);
  assert (buf[1] == 0x34);

  return 0;
}
