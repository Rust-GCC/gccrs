/* { dg-do compile } */
/* { dg-options "-fopenmp -fdump-tree-gimple" } */

void f1(void)
{
  #pragma omp barrier
}

void f2(bool p)
{
  if (p)
    {
      #pragma omp barrier
    }
}

/* { dg-final { scan-tree-dump-times "GOMP_barrier_ext \\(2\\)" 2 "gimple" } } */
