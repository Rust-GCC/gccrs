/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh -mabi=lp64d -mcpu=xt-c9501fdvt -O3 -mrvv-vector-bits=scalable -fdump-tree-vect-details" } */

void
add1_i32 (int *restrict a, int *restrict b, int n)
{
  /* b[i] + 1 prevents the loop from being recognised as memcpy.  */
  for (int i = 0; i < n; ++i)
    a[i] = b[i] + 1;
}

/* { dg-final { scan-tree-dump-times "optimized: loop vectorized" 1 "vect" } } */
/* { dg-final { scan-tree-dump "vector_load costs 6 in body" "vect" } } */
/* { dg-final { scan-tree-dump "vector_store costs 1 in body" "vect" } } */
