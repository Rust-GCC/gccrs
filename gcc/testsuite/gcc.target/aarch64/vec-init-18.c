/* { dg-do compile } */
/* { dg-options "-O3" } */

#include <arm_neon.h>

int16x8_t foo(int16_t x, int y)
{
  int16x8_t v = (int16x8_t) {x, y, x, y, x, y, x, y}; 
  return v;
}

int16x8_t foo2(int16_t x) 
{
  int16x8_t v = (int16x8_t) {x, 1, x, 1, x, 1, x, 1}; 
  return v;
}

/* { dg-final { scan-assembler-times {\tmov\tw1, 1} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tv0+\.4s, w0} 2 } } */
/* { dg-final { scan-assembler-times {\tbfi\tw0, w1, 16, 16} 2 } } */
