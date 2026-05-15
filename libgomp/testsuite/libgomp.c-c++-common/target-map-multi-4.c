/* Test multiple map clauses for the same variable in target enter/exit data
   constructs, including release and delete map-types.  */
/* { dg-do run } */

#define N 100

int
main (void)
{
  int a[N];
  int sum;

  /* delete + release  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target enter data map(alloc: a) map(to: a)
  sum = 0;
  #pragma omp target map(alloc: a) map(tofrom: sum)
    for (int i = 0; i < N; i++)
      sum += a[i];
  if (sum != N * (N - 1) / 2)
    __builtin_abort ();
  #pragma omp target exit data map(delete: a) map(release: a)


  /* release + release: duplicate release
     decrements the reference count once (deduplicated).  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target enter data map(to: a)   /* refcount = 1 */
  #pragma omp target enter data map(to: a)   /* refcount = 2 */
  #pragma omp target exit data map(release: a) map(release: a)  /* refcount = 1 */
  sum = 0;
  #pragma omp target map(alloc: a) map(tofrom: sum)
    for (int i = 0; i < N; i++)
      sum += a[i];
  if (sum != N * (N - 1) / 2)
    __builtin_abort ();
  #pragma omp target exit data map(delete: a)   /* refcount = 0 */

  /* delete + delete: duplicate delete
     removes the mapping unconditionally once (deduplicated).  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target enter data map(to: a)
  #pragma omp target exit data map(delete: a) map(delete: a)

  /* from + release: copy device values back
     to host and release the mapping.  */
  for (int i = 0; i < N; i++)
    a[i] = i;
  #pragma omp target enter data map(to: a)   /* refcount = 1 */
  #pragma omp target
    for (int i = 0; i < N; i++)
      a[i] *= 3;
  #pragma omp target exit data map(release: a) map(from: a)  /* refcount = 0 */
  for (int i = 0; i < N; i++)
    if (a[i] != i * 3)
      __builtin_abort ();

  for (int i = 0; i < N; i++)
    if (a[i] != i * 3)
      __builtin_abort ();

  return 0;
}
