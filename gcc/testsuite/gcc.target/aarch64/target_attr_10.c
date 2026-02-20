/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+simd -mcpu=generic" } */

/* Using a SIMD intrinsic from a function tagged with nosimd should fail
   due to inlining rules.  */

#include "arm_neon.h"

__attribute__ ((target ("+nosimd")))
uint8x16_t
foo (uint8x16_t a, uint8x16_t b, uint8x16_t c)
{
  return vbslq_u8 (a, b, c); /* { dg-error {ACLE function 'vbslq_u8' requires ISA extension 'simd'} } */
}
