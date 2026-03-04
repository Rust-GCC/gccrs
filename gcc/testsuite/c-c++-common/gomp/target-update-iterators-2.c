/* { dg-do compile } */
/* { dg-options "-fopenmp -fdump-tree-gimple" } */

void f (int *x, float *y, double *z)
{
  #pragma omp target update to(iterator(i=0:10): x)
    ;

  #pragma omp target update from(iterator(i2=0:10, j2=0:20): x[i2])
    ;

  #pragma omp target update to(iterator(i3=0:10, j3=0:20, k3=0:30): x[i3+j3], y[j3+k3], z[k3+i3])
    ;
}

/* { dg-final { scan-tree-dump "update to\\\(x " "gimple" } } */
/* { dg-final { scan-tree-dump "update from\\\(iterator\\\(int i2=0:10:1, loop_label=" "gimple" } } */
/* { dg-final { scan-tree-dump "to\\\(iterator\\\(int i3=0:10:1, int k3=0:30:1, loop_label=" "gimple" } } */
/* { dg-final { scan-tree-dump "to\\\(iterator\\\(int j3=0:20:1, int k3=0:30:1, loop_label=" "gimple" } } */
/* { dg-final { scan-tree-dump "to\\\(iterator\\\(int i3=0:10:1, int j3=0:20:1, loop_label=" "gimple" } } */
