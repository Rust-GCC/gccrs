/* { dg-do run { target { riscv_v } } } */
/* { dg-options "-O3 -march=rv64gcv -mabi=lp64d -fno-vect-cost-model" } */

#include <stdint-gcc.h>

__attribute__((noipa)) 
void foo(uint8_t *d) 
{
  d[0] = 128; d[1] = 135; d[2] = 130; d[3] = 149;
  d[4] = 132; d[5] = 163; d[6] = 134; d[7] = 177;
  d[8] = 136; d[9] = 191; d[10] = 138; d[11] = 205;
  d[12] = 140; d[13] = 219; d[14] = 142; d[15] = 233;
}

int main()
{
  uint8_t d[16];
  uint8_t e[] = {128, 135, 130, 149, 132, 163, 134, 177, 136, 191, 138, 205, 140, 219, 142, 233};
  foo(d);
  for (int i = 0; i < 16; i++)
    if (d[i] != e[i])
      __builtin_abort ();
  return 0;
}
