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

/* { dg-final { scan-assembler-times {\tdup\tv[0-9]+\.4s, v[0-9]+\.s\[0\]} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tv[0-9]+\.4s, w[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tw[0-9]+, 65537} 1 } } */
/* { dg-final { scan-assembler-times {\tbfi\tw[0-9]+, w[0-9]+, 0, 16} 1 } } */
/* { dg-final { scan-assembler-times {\tbfi\tw[0-9]+, w[0-9]+, 16, 16} 1 } } */
