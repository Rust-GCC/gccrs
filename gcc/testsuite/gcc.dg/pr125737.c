/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

int
f (unsigned short a, unsigned short b) {
  int aa = a;
  int bb = b;
  int c = aa % bb;
  return c < bb;
}

/* { dg-final { scan-tree-dump-times "return 1;" 1 "optimized" } } */
