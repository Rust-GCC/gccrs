/* { dg-do compile } */
/* { dg-options "-O3 -mavx512f -mtune=c86-4g-m7 --param ix86-vect-compare-costs=1 -fdump-tree-vect-details" } */

int
find_ll (const unsigned long long *p, unsigned long long x, int n)
{
  for (int i = 0; i < n; ++i)
    if (p[i] == x)
      return i;
  return -1;
}

/* The compare cost should make c86-4g-m7 prefer 32-byte vectors.  */
/* { dg-final { scan-tree-dump "loop vectorized using 32 byte vectors" "vect" } } */
