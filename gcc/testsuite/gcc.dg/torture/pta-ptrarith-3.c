/* { dg-do run } */
/* { dg-options "-fdump-tree-alias" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-fno-fat-lto-objects" } { "" } } */

extern void abort (void);
struct X {
  int *p;
  int *q;
  int *r;
};
int __attribute__((noinline))
foo(int i, int j, int k, __SIZE_TYPE__ off)
{
  struct X x;
  int **p, *q;
  x.p = &i;
  x.q = &j;
  x.r = &k;
  p = &x.q;
  p += off;
  /* *p points to { i, j, k } */
  q = *p;
  return *q;
}
int main()
{
  if (foo(1, 2, 3, -1) != 1)
    abort ();
  if (foo(1, 2, 3, 0) != 2)
    abort ();
  if (foo(1, 2, 3, 1) != 3)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump "q_\[0-9\]* = { i j k }" "alias" } } */
