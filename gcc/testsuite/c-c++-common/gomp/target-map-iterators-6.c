/* { dg-do compile } */
/* { dg-options "-fopenmp -fdump-tree-gimple" } */

#define DIM 17

void f (int **x)
{
    #pragma omp target data map(iterator(it = 0:DIM), tofrom: x[it])
      {
        for (int i = 0; i < DIM; i++)
          *x[i] += i;
      }
}

/* { dg-final { scan-tree-dump "if \\(i <= 16\\) goto <D\\\.\[0-9\]+>; else goto <D\\\.\[0-9\]+>;" "gimple" } } */
/* { dg-final { scan-tree-dump "map\\(iterator\\(int it=0:17:1, .+\\):tofrom:\\*D\\\.\[0-9\]+" "gimple" } } */
/* { dg-final { scan-tree-dump "map\\(iterator\\(int it=0:17:1, .+\\):attach:x" "gimple" } } */
