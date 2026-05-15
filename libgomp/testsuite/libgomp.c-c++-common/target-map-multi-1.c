/* Test multiple map clauses for the same variable with various
   combinations of map-types: alloc, to, from, tofrom.  */
/* { dg-do run } */

#define N 100

int
main (void)
{
  int a[N];
  int sum;

  /* map(to) + map(from) = map(tofrom).  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target map(to: a) map(alloc: a) map(from: a)
    for (int i = 0; i < N; i++)
      a[i] *= 4;
  for (int i = 0; i < N; i++)
    if (a[i] != i * 4)
      __builtin_abort ();

  /* map(to) + map(tofrom): tofrom covers both directions.  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target map(to: a) map(tofrom: a) map(to: a)
    for (int i = 0; i < N; i++)
      a[i] *= 5;
  for (int i = 0; i < N; i++)
    if (a[i] != i * 5)
      __builtin_abort ();

  /* map(from) + map(tofrom): tofrom covers both directions.  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target map(from: a) map(tofrom: a) map(from: a)
    for (int i = 0; i < N; i++)
      a[i] *= 6;
  for (int i = 0; i < N; i++)
    if (a[i] != i * 6)
      __builtin_abort ();

  /* map(alloc) + map(to): device gets host values via 'to'.  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  sum = 0;
  #pragma omp target map(alloc: a) map(to: a) map(tofrom: sum) map(alloc: sum)
    for (int i = 0; i < N; i++)
      sum += a[i];
  if (sum != N * (N - 1) / 2)
    __builtin_abort ();

  /* map(alloc) + map(from): device values come back to host via 'from'.  */
  #pragma omp target map(alloc: a) map(from: a) map(alloc: a)
    for (int i = 0; i < N; i++)
      a[i] = i * 7;
  for (int i = 0; i < N; i++)
    if (a[i] != i * 7)
      __builtin_abort ();

  /* map(alloc) + map(tofrom): full bidirectional transfer.  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target map(alloc: a) map(tofrom: a) map(alloc: a)
    for (int i = 0; i < N; i++)
      a[i] *= 8;
  for (int i = 0; i < N; i++)
    if (a[i] != i * 8)
      __builtin_abort ();

  return 0;
}
