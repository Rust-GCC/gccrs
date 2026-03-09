/* { dg-do compile { target avx2 } } */
/* { dg-options "-O2 -mavx2 -mno-avx512f -fdump-tree-vect-details" } */

void f (short* acc)
{
  for (unsigned char row = 0; row < 16; ++row)
    acc[row] = acc[row] << row;
}

/* { dg-final { scan-tree-dump "optimized: loop vectorized" "vect" } } */
