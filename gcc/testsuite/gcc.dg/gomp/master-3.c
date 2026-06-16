/* { dg-do compile } */
/* { dg-options "-fopenmp -fdump-tree-ompexp -Wno-deprecated-openmp" } */

extern void bar(int);

void foo (void)
{
  #pragma omp master
    bar(0);
}

/* { dg-final { scan-tree-dump-times "GOMP_has_masked_thread_num" 1 "ompexp" } } */
