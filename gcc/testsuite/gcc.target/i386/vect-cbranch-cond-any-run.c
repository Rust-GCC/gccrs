/* { dg-do run { target { avx512vl && avx512dq } } } */
/* { dg-options "-Ofast -mavx512f -mavx512vl -mavx512dq -mprefer-vector-width=128" } */

#define AVX512VL
#define AVX512DQ
#define DO_TEST test_cbranch_any
static void test_cbranch_any (void);
#include "avx512-check.h"

#define N 3   /* V2DI nelem (2) + 1 */
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

static void
test_cbranch_any (void)
{
  /* a[0] != d AND c[0] != e  → break immediately, only b[0] updated.  */
  reset ();
  a[0] = 5;  c[0] = 5;
  a[1] = 1;  c[1] = 7;
  a[2] = 1;  c[2] = 7;
  f (1 /*d*/, 7 /*e*/);
  CHECK (0, 5); CHECK (1, 0); CHECK (2, 0);

  /* All a[i]==d  → continue cond never breaks.  All updated.  */
  reset ();
  a[0] = 1;  c[0] = 7;
  a[1] = 1;  c[1] = 7;
  a[2] = 1;  c[2] = 7;
  f (1, 7);
  CHECK (0, 1); CHECK (1, 1); CHECK (2, 1);

  /* All c[i]==e  → outer cond hits "continue" each iter, no break.  */
  reset ();
  a[0] = 5;  c[0] = 7;     /* c==e (e=7) — continue */
  a[1] = 6;  c[1] = 7;
  a[2] = 8;  c[2] = 7;
  f (1, 7);
  CHECK (0, 5); CHECK (1, 6); CHECK (2, 8);

  /* break at index 1.  */
  reset ();
  a[0] = 1;  c[0] = 0;     /* a==d, no break */
  a[1] = 9;  c[1] = 0;     /* a!=d AND c!=e → break here */
  a[2] = 0;  c[2] = 0;
  f (1, 7);
  CHECK (0, 1); CHECK (1, 9); CHECK (2, 0);
}
