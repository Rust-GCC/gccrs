/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized -Wno-psabi" } */

typedef int v4si __attribute__((vector_size(4 * sizeof(int))));

int
f (int a)
{
  return a * 266 + (a << 12);
}


v4si
f_vec (v4si a)
{
  return a * 266 + (a << 12);
}

int
g (int a)
{
  int t = (a << 7) + a;
  return t + (a << 12);
}

/* { dg-final { scan-tree-dump-not "<<" "optimized" } } */
