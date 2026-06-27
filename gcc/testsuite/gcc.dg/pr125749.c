/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

_Bool
max_eq_zero (unsigned short a, unsigned short b) {
  int aa = a;
  int bb = b;
  int t = aa > bb ? aa : bb;
  return (t == 0) == ((a | b) == 0);
}

_Bool
max_ne_zero (unsigned short a, unsigned short b) {
  int aa = a;
  int bb = b;
  int t = aa > bb ? aa : bb;
  return (t != 0) == ((a | b) != 0);
}

/* { dg-final { scan-tree-dump-times "return 1;" 2 "optimized" } } */
