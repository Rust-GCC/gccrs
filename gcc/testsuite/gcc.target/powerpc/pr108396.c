/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-require-effective-target int128 } */
/* { dg-options "-mdejagnu-cpu=power8" } */

/* Verify there is no error message.  */

#include <altivec.h>

vector unsigned __int128
vsubcuq (vector unsigned __int128 a, vector unsigned __int128 b)
{
  return vec_vsubcuq (a, b);
}

