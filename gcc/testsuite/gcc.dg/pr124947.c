/* { dg-options "-O -fdump-tree-ivopts" } */

long *BN_uadd_ap;

void
BN_uadd (int dif, long t1)
{
  volatile long *rp;
  while (dif)
    {
      dif--;
      t1 = *BN_uadd_ap;
      *rp++ = t1;
      if (t1)
        break;
    }
}

/* { dg-final { scan-tree-dump-times "rp_\[0-9\]+\\\(D\\\)" 1 "ivopts" } } */
