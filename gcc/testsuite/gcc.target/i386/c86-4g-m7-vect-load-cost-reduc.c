/* { dg-do compile } */
/* { dg-options "-O3 -march=c86-4g-m7 -mprefer-vector-width=512 -ffast-math -fdump-tree-vect-details" } */

#define N 1024

void foo (double *__restrict a, double *__restrict b, double *out)
{
  double sum = 1.0;

  for (int i = 0; i < N; ++i)
    sum += a[i] * b[i];

  out[0] = sum;
}

/* On c86-4g-m7, 512-bit memory costs should model two 256-bit operations on
   split-regs targets.  This loop exercises the load side of that model: once
   the normalized body cost no longer favors 64-byte vectors, the reduction
   epilogue cost makes 32-byte vectorization preferable.  */
/* { dg-final { scan-tree-dump-times "optimized: loop vectorized using 32 byte vectors" 1 "vect" } } */
/* { dg-final { scan-tree-dump-not "optimized: loop vectorized using 64 byte vectors" "vect" } } */
