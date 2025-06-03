/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target ilp32 } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

/* This test should succeed only on 32-bit configurations.  */

int
test_byte_in_set (unsigned char b, unsigned long long set_members)
{
  return __builtin_byte_in_set (b, set_members); /* { dg-error "'__builtin_scalar_byte_in_set' requires the" } */
}
