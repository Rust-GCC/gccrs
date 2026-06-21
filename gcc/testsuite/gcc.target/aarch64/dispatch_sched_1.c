/* Check DFA lookahead is not reached when dispatch scheduling is enabled.  */
/* { dg-do compile } */
/* { dg-options "-O3 -mcpu=neoverse-v2 -fdump-rtl-sched2-details -fsched-verbose=2" } */

void
foo (int *restrict a, int *restrict b, int *restrict c, int n)
{
#pragma GCC unroll 64
  for (int i = 0; i < n; i++)
    c[i] += a[i] * b[i];
}

/* { dg-final { scan-rtl-dump-not "max_issue among" "sched2" } } */
