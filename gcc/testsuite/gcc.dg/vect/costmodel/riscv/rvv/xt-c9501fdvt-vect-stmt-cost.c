/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh -mabi=lp64d -mcpu=xt-c9501fdvt -O3 -mrvv-vector-bits=scalable -fdump-tree-vect-details" } */

void
add_i32 (int *restrict a, int *restrict b, int n)
{
  for (int i = 0; i < n; ++i)
    a[i] += b[i];
}

void
add_f32 (float *restrict a, float *restrict b, int n)
{
  for (int i = 0; i < n; ++i)
    a[i] += b[i];
}

/* { dg-final { scan-tree-dump-times "optimized: loop vectorized" 2 "vect" } } */
/* { dg-final { scan-tree-dump "vector_stmt costs 3 in body" "vect" } } */
/* { dg-final { scan-tree-dump "vector_stmt costs 4 in body" "vect" } } */
