/* { dg-do run } */
/* { dg-require-effective-target aarch64_variant_pcs } */
/* { dg-options "-O1 -march=armv8-a" } */

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <arm_sme.h>

unsigned char basic[] = {'a', 'b', 'c', 'd', 'e', 'f'};

int
main (void)
{
  unsigned char buffer[1024];
  memset (buffer, 0xaa, sizeof buffer);
  buffer[128] = 'x';

  assert (__arm_sc_memchr (basic, 'a', sizeof basic) == basic);
  assert (__arm_sc_memchr (basic, 'd', sizeof basic) == basic + 3);
  assert (__arm_sc_memchr (basic, 'f', sizeof basic) == basic + 5);
  assert (__arm_sc_memchr (basic, 'x', sizeof basic) == NULL);

  assert (__arm_sc_memchr (basic, 'd', 3) == NULL);
  assert (__arm_sc_memchr (basic, 'd', 4) == basic + 3);
  assert (__arm_sc_memchr (basic, 'a', 0) == NULL);

  assert (__arm_sc_memchr (basic, 'a' + 256, sizeof basic) == basic);

  assert (__arm_sc_memchr (basic + 1, 'a', sizeof (basic) - 1) == NULL);

  assert (__arm_sc_memchr (buffer, 'x', sizeof buffer) == buffer + 128);

  assert (__arm_sc_memchr (buffer, 'x', SIZE_MAX) == buffer + 128);

  return 0;
}
