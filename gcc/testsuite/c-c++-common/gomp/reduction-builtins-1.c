/* { dg-do compile } */
/* { dg-additional-options "-fdump-tree-omplower" } */

/* Check that, for reductions, critical sections are bracketed with
   GOMP_reduction_start/end rather than GOMP_atomic_start/end. Two scalar
   reductions force the critical-section path (a single scalar reduction would
   use an atomic update instead).  */

void foo (int);

void r_for (int n) {
  int a = 0, b = 0, i;
  #pragma omp parallel
  #pragma omp for reduction(+: a, b)
  for (i = 0; i < n; i++) { a += i; b += i; }
}

void r_sections (void) {
  int a = 0, b = 0;
  #pragma omp parallel
  #pragma omp sections reduction(+: a, b)
  { foo (a + b); }
}

void r_scope (void) {
  int a = 0, b = 0;
  #pragma omp parallel
  #pragma omp scope reduction(+: a, b)
  foo (a + b);
}

void r_teams (void) {
  int a = 0, b = 0;
  #pragma omp teams reduction(+: a, b)
  foo (a + b);
}

void r_parallel (void) {
  int a = 0, b = 0;
  #pragma omp parallel reduction(+: a, b)
  foo (a + b);
}

/* { dg-final { scan-tree-dump-times "GOMP_reduction_start \\(" 5 "omplower" } } */
/* { dg-final { scan-tree-dump-times "GOMP_reduction_end \\(" 5 "omplower" } } */
/* { dg-final { scan-tree-dump-not "GOMP_atomic_start" "omplower" } } */
/* { dg-final { scan-tree-dump-not "GOMP_atomic_end" "omplower" } } */
