/* { dg-do compile } */
/* { dg-require-effective-target vect_int } */
/* { dg-additional-options "-O3 -fdump-tree-slp2" } */

/* Test that SLP recognizes "sum of diffs" (d_i = a[i]-b[i], sum = d0+...+d7)
   as a reduction chain and vectorizes the basic block.  */

int __attribute__((noipa))
sum_of_diffs (int *a, int *b)
{
  int d0 = a[0] - b[0];
  int d1 = a[1] - b[1];
  int d2 = a[2] - b[2];
  int d3 = a[3] - b[3];
  int d4 = a[4] - b[4];
  int d5 = a[5] - b[5];
  int d6 = a[6] - b[6];
  int d7 = a[7] - b[7];

  int sum = d0 + d1 + d2 + d3 + d4 + d5 + d6 + d7;
  return sum;
}

/* Check that the reduction was SLP-vectorized (REDUC_PLUS appears in the tree).  */
/* { dg-final { scan-tree-dump "\.REDUC_PLUS" "slp2" { target x86_64-*-* aarch64*-*-* } } } */
