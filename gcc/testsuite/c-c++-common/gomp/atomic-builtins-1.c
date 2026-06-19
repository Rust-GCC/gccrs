/* { dg-do compile } */
/* { dg-additional-options "-fdump-tree-ompexp" } */

/* Check that an GIMPLE_OMP_ATOMIC statement expands to GOMP_atomic_start/end
   when atomic operations cannot be used and a mutex is required.  */

void bar (__int128 a, __int128 b) {
  #pragma omp atomic capture
    b = a++;
}

/* { dg-final { scan-tree-dump "GOMP_atomic_start" "ompexp" } } */
/* { dg-final { scan-tree-dump "GOMP_atomic_end" "ompexp" } } */
