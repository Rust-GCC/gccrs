/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target ilp32 } */
/* { dg-require-effective-target powerpc_vsx } */

#include <stddef.h>
#include <altivec.h>

/* This test only runs on 32-bit configurations, where a compiler
   error should be issued because this built-in function is not
   available on 32-bit configurations. */

int
fetch_data (float *address, size_t length)
{
  return __builtin_vec_lxvl (address, length);	/* { dg-error "'__builtin_vsx_lxvl' requires the" } */
}
