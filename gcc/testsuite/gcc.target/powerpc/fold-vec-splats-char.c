/* Verify that overloaded built-ins for vec_splats() with char
   inputs produce the right code.  */

/* { dg-do compile } */
/* { dg-options "-maltivec -O2 " } */
/* { dg-require-effective-target powerpc_altivec } */

#include <altivec.h>

vector signed char
test1s (signed char x)
{
  return vec_splats (x);
}

vector unsigned char
test1u (unsigned char x)
{
  return vec_splats (x);
}

/* { dg-final { scan-assembler-times "vspltb" 2 } } */
