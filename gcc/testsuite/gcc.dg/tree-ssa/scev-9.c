/* { dg-do compile } */
/* { dg-options "-O2 -fno-tree-vectorize -fdump-tree-ivopts-details" } */

int *a;

int
foo (unsigned char s, unsigned char l)
{
  unsigned char i;
  int sum = 0;

  for (i = s; i < l; i += 1)
    {
      sum += a[i];
    }

  return sum;
}

/* Address of array reference is scev.  */
/* { dg-final { scan-tree-dump-times "  Type:\\tREFERENCE ADDRESS\n" 1 "ivopts" } } */

