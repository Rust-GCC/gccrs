/* { dg-do compile } */
/* { dg-options "-O3 -fdump-tree-unswitch-optimized" } */

extern volatile int sink;

void
foo (int flag, int n, int m)
{
  for (int i = 0; i < n; ++i)
    {
      /* Pad the outer loop to make it noticeably larger than the inner.  */
      sink += i;
      sink += 2;
      sink += 3;
      sink += 4;
      sink += 5;
      sink += 6;
      sink += 7;
      sink += 8;
      sink += 9;
      sink += 10;
      sink += 11;
      sink += 12;
      sink += 13;
      sink += 14;
      sink += 15;
      sink += 16;
      sink += 17;
      sink += 18;
      sink += 19;
      sink += 20;

      for (int j = 0; j < m; ++j)
        if (flag)
          sink += j;
    }
}

/* We should still unswitch, but stay on the inner loop because the
   outer-only body (the part that would be duplicated by hoisting the
   unswitching) exceeds param_max_unswitch_insns.  */
/* { dg-final { scan-tree-dump "unswitching loop" "unswitch" } } */
/* { dg-final { scan-tree-dump-not "unswitching outer loop" "unswitch" } } */
