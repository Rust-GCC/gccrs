/* { dg-do run { target { avx512vl && avx512dq } } } */
/* { dg-options "-Ofast -mavx512f -mavx512vl -mavx512dq -mprefer-vector-width=128" } */

#define AVX512VL
#define AVX512DQ
#define DO_TEST test_cbranch_all
static void test_cbranch_all (void);
#include "avx512-check.h"

#define N 100
long long a[N];
long long b[N];
long long c[N];

__attribute__((noipa))
void f (long long d, long long e)
{
  for (int i = 0; i < N; i++)
    {
      b[i] += a[i];
      if (c[i] == e) continue;
      if (a[i] != d) break;
    }
}

static void reset (void)
{
  for (int i = 0; i < N; i++) { a[i] = 0; b[i] = 0; c[i] = 0; }
}

#define CHECK(_i, _v) do { if (b[_i] != (_v)) __builtin_abort (); } while (0)
#define CHECK_RANGE_EQ(_lo, _hi, _v)         \
  do {                                       \
    for (int i = _lo; i < _hi; i++)          \
      if (b[i] != (_v)) __builtin_abort ();  \
  } while (0)

static void
test_cbranch_all (void)
{
  /* No break — all a[i] == d.  Whole loop runs; b[i] = a[i].  */
  reset ();
  for (int i = 0; i < N; i++) { a[i] = 7; c[i] = 0; }
  f (7 /*d*/, 99 /*e*/);
  CHECK_RANGE_EQ (0, N, 7);

  /* Break at index 50: a[i]==d for i<50, a[50]!=d AND c[50]!=e.  */
  reset ();
  for (int i = 0; i < N; i++) { a[i] = 7; c[i] = 0; }
  a[50] = 99;
  f (7, 5);
  CHECK_RANGE_EQ (0, 50, 7); CHECK (50, 99);
  CHECK_RANGE_EQ (51, N, 0);

  /* "continue" gates the break: c[50]==e at the same i where a[i]!=d
     should *not* break, loop keeps going.  */
  reset ();
  for (int i = 0; i < N; i++) { a[i] = 7; c[i] = 0; }
  a[50] = 99;  c[50] = 5;     /* c==e -> continue, skip break check */
  f (7, 5);
  CHECK (50, 99);
  for (int i = 0; i < N; i++)
    if (i != 50 && b[i] != 7) __builtin_abort ();

  /* Break at index 0.  */
  reset ();
  a[0] = 99;  c[0] = 0;       /* a!=d AND c!=e at i=0 */
  for (int i = 1; i < N; i++) { a[i] = 7; c[i] = 0; }
  f (7, 5);
  CHECK (0, 99);
  CHECK_RANGE_EQ (1, N, 0);
}
