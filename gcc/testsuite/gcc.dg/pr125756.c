/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

bool
f1 (unsigned short aa)
{
  int a = aa;
  bool t = a != 0;
  int tu = t;
  return a < tu;
}

bool
f2 (unsigned short a)
{
  bool t = a != 0;
  unsigned short tu = t;
  return a < tu;
}

/* { dg-final { scan-tree-dump-times "return 0;" 2 "optimized" } } */
