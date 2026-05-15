// Test multiple map clauses for the same variable using pointer array sections
// and C++ references.
// { dg-do run }

extern "C" void abort ();

static const int N = 100;

/* Test fixed-size array with multiple map clauses.  */
static void
test_pointer ()
{
  int p[N];

  /* map(to) + map(alloc) + map(from): three clauses on the same array.  */
  for (int i = 0; i < N; i++) p[i] = i;
  #pragma omp target map(to: p) map(alloc: p) map(from: p)
    for (int i = 0; i < N; i++) p[i] *= 4;
  for (int i = 0; i < N; i++) if (p[i] != i * 4) abort ();

  /* map(to) + map(tofrom): tofrom covers both directions.  */
  for (int i = 0; i < N; i++) p[i] = i;
  #pragma omp target map(to: p) map(tofrom: p)
    for (int i = 0; i < N; i++) p[i] *= 5;
  for (int i = 0; i < N; i++) if (p[i] != i * 5) abort ();

  /* map(alloc) + map(to): device gets host values via 'to'.  */
  for (int i = 0; i < N; i++) p[i] = i;
  int sum = 0;
  #pragma omp target map(alloc: p) map(to: p) map(tofrom: sum)
    for (int i = 0; i < N; i++) sum += p[i];
  if (sum != N * (N - 1) / 2) abort ();

  /* map(alloc) + map(from): device values come back via 'from'.  */
  #pragma omp target map(alloc: p) map(from: p)
    for (int i = 0; i < N; i++) p[i] = i * 7;
  for (int i = 0; i < N; i++) if (p[i] != i * 7) abort ();

  /* map(alloc) + map(tofrom) + map(alloc): three clauses, full bidirectional.  */
  for (int i = 0; i < N; i++) p[i] = i;
  #pragma omp target map(alloc: p) map(tofrom: p) map(alloc: p)
    for (int i = 0; i < N; i++) p[i] *= 8;
  for (int i = 0; i < N; i++) if (p[i] != i * 8) abort ();
}

/* Test C++ array references: r aliases a.  */
static void
test_reference ()
{
  int a[N];
  int (&r)[N] = a;

  /* map(to: r) + map(from: r): send and receive via the same reference.  */
  for (int i = 0; i < N; i++) a[i] = i;
  #pragma omp target map(to: r) map(from: r)
    for (int i = 0; i < N; i++) r[i] *= 4;
  for (int i = 0; i < N; i++) if (r[i] != i * 4) abort ();

  /* map(to: r) + map(tofrom: r): to and tofrom on the same reference.  */
  for (int i = 0; i < N; i++) a[i] = i;
  #pragma omp target map(to: r) map(tofrom: r)
    for (int i = 0; i < N; i++) r[i] *= 5;
  for (int i = 0; i < N; i++) if (r[i] != i * 5) abort ();

  /* map(to: r) + map(alloc: r) + map(from: r): three clauses on the same reference.  */
  for (int i = 0; i < N; i++) a[i] = i;
  #pragma omp target map(to: r) map(alloc: r) map(from: r)
    for (int i = 0; i < N; i++) r[i] *= 6;
  for (int i = 0; i < N; i++) if (r[i] != i * 6) abort ();

  /* map(alloc: r) + map(to: r): alloc and to on the same reference.  */
  for (int i = 0; i < N; i++) a[i] = i;
  int sum = 0;
  #pragma omp target map(alloc: r) map(to: r) map(tofrom: sum)
    for (int i = 0; i < N; i++) sum += r[i];
  if (sum != N * (N - 1) / 2) abort ();
}

/* Test pointer + array sections with enter/exit data using multiple map
   clauses.  */
static void
test_pointer_enter_exit ()
{
  int *p = new int[N];

  /* map(alloc) + map(to) on enter data.  */
  for (int i = 0; i < N; i++) p[i] = i;
  #pragma omp target enter data map(alloc: p[0:N]) map(to: p[0:N])
  int sum = 0;
  #pragma omp target map(alloc: p[0:N]) map(tofrom: sum)
    for (int i = 0; i < N; i++) sum += p[i];
  if (sum != N * (N - 1) / 2) abort ();
  #pragma omp target exit data map(delete: p[0:N])

  /* map(release) + map(from) on exit data: copy back then release.  */
  for (int i = 0; i < N; i++) p[i] = i;
  #pragma omp target enter data map(to: p[0:N])
  #pragma omp target map(alloc: p[0:N])
    for (int i = 0; i < N; i++) p[i] *= 3;
  #pragma omp target exit data map(release: p[0:N]) map(from: p[0:N])
  for (int i = 0; i < N; i++) if (p[i] != i * 3) abort ();

  delete[] p;
}

int
main ()
{
  test_pointer ();
  test_reference ();
  test_pointer_enter_exit ();
  return 0;
}
