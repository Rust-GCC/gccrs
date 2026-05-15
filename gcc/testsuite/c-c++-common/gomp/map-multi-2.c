/* { dg-do compile } */
/* { dg-additional-options "-fdump-tree-original" } */

/* Check that map clauses are only merged if they have the same modifiers.  */

#define DIM 17

void f (int *x)
{
  #pragma omp target map(always, to: x) map(tofrom: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
  #pragma omp target map(tofrom: x) map(always, from: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
  #pragma omp target map(to: x) map(present, alloc: x) map(from: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
  #pragma omp target map(close,always,from: x) map(close,present,alloc: x) map(present,to: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
  #pragma omp target map(always,to: x) map(always,present,alloc: x) map(present,from: x)
    {
      for (int i = 0; i < DIM; i++)
        x[i] += i;
    }
}

/* { dg-final { scan-tree-dump-times {map\(tofrom:x\) map\(always,to:x\)} 1 "original" } } */
/* { dg-final { scan-tree-dump-times {map\(always,from:x\) map\(tofrom:x\)} 1 "original" } } */
/* { dg-final { scan-tree-dump-times {map\(present,alloc:x\) map\(tofrom:x\)} 1 "original" } } */
/* { dg-final { scan-tree-dump-times {map\(present,to:x\) map\(always,from:x\)} 1 "original" } } */
/* { dg-final { scan-tree-dump-times {map\(present,from:x\) map\(always,to:x\)} 1 "original" } } */
