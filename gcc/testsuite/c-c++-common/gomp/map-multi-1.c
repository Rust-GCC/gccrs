/* { dg-do compile } */
/* { dg-additional-options "-fdump-tree-original" } */

/* Check that extraneous clauses mapping the same variable multiple times are
   either removed or merged.  */

#define DIM 17

void f (int *x)
{
  #pragma omp target map(alloc: x) map(to: x) map(alloc: x) map(from: x) map(alloc: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
  #pragma omp target map(alloc: x) map(alloc: x) map(alloc: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
  #pragma omp target map(alloc: x) map(to: x) map(alloc: x) map(to: x) map(alloc: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
  #pragma omp target map(alloc: x) map(from: x) map(alloc: x) map(from: x) map(alloc: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
}

/* { dg-final { scan-tree-dump-times {map\(tofrom:x\)} 1 "original" } } */
/* { dg-final { scan-tree-dump-times {map\(alloc:x\)} 1 "original" } } */
/* { dg-final { scan-tree-dump-times {map\(to:x\)} 1 "original" } } */
/* { dg-final { scan-tree-dump-times {map\(from:x\)} 1 "original" } } */
