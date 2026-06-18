/* { dg-do compile } */
/* { dg-additional-options "-fdump-tree-ompexp" } */

/* Check that implicit barriers are emitted with the correct kind.  */

void body (void);
void bar (int);
void foo (int);

/* OMP for construct with static schedule (with and without chunk) -> workshare
   barrier.  */
void wfor (int n) {
  int i;
  #pragma omp for schedule(static)
  for (i = 0; i < n; i++) bar (i);
  #pragma omp for schedule(static, 4)
  for (i = 0; i < n; i++) bar (i);
}

/* OMP for construct with inscan reduction -> workshare barrier.  */
int r;
void wfor_scan (int n)
{
  int i;
  #pragma omp for reduction(inscan, + : r) private(i)
  for (i = 0; i < n; i = i + 1)
    {
      {
	bar (r);
      }
      #pragma omp scan inclusive(r)
      {
	foo (r);
      }
    }
}

/* OMP sections construct with firstprivate and lastprivate clauses -> workshare
   barrier.  */
int a;
int wsections (void) {
  #pragma omp sections firstprivate(a) lastprivate(a)
  { foo (a); }
  return a;
}

/* parallel with a copyin variable passed by reference (an aggregate, so
   use_pointer_for_field is true) -> copyin_by_ref -> parallel barrier.  */
int tp[64];
#pragma omp threadprivate (tp)
void pcopyin (void) {
  #pragma omp parallel copyin(tp)
  body ();
}

/* UDR whose initializer references omp_orig, used on a (non-task) parallel
   construct -> reduction_omp_orig_ref -> parallel barrier.  */
struct S { int s; };
void combine (struct S *out, struct S *in);
struct S init_from_orig (struct S *orig);
void use (struct S *);

#pragma omp declare reduction (+: struct S: combine (&omp_out, &omp_in)) \
	initializer (omp_priv = init_from_orig (&omp_orig))

void preduction_orig (struct S s) {
  #pragma omp parallel reduction (+: s)
  use (&s);
}

/* Same omp_orig UDR on a teams construct -> parallel barrier.  */
void treduction_orig (struct S s) {
  #pragma omp teams reduction (+: s)
  use (&s);
}

/* Same omp_orig UDR on a scope construct -> workshare barrier.  */
struct S gs;
void sreduction_orig (void) {
  #pragma omp parallel
  #pragma omp scope reduction (+: gs)
  use (&gs);
}

/* OMP single construct (without nowait) -> workshare barrier.  */
void wsingle (void) {
  #pragma omp single
  body ();
}

/* OMP scope construct (without nowait) -> workshare barrier.  */
void wscope (void) {
  #pragma omp scope
  body ();
}

/* GOMP_BARRIER_IMPLICIT_PARALLEL (0): pcopyin, preduction_orig,
   treduction_orig.  */
/* { dg-final { scan-tree-dump-times "GOMP_barrier_ext \\(0\\)" 3 "ompexp" } } */
/* GOMP_BARRIER_IMPLICIT_WORKSHARE (1): wfor (x2), wfor_scan (x2), wsections,
   sreduction_orig, wsingle, wscope.  */
/* { dg-final { scan-tree-dump-times "GOMP_barrier_ext \\(1\\)" 8 "ompexp" } } */
