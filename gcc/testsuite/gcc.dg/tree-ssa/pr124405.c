/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-cselim-details" } */

void
f (int *a, int b)
{
  *a &= ~3;
  if (b)
    *a |= 1;
}

/* { dg-final { scan-tree-dump-times "Conditional store replacement happened" 1 "cselim"} } */
