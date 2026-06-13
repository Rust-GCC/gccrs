/* { dg-do run } */
/* { dg-additional-options "-fgimple -fdump-tree-optimized" } */
/* { dg-skip-if "" { *-*-* } { "-fno-fat-lto-objects" } { "" } } */
/* PR tree-optimization/125774 */

__attribute__((noipa))
_Bool __GIMPLE (ssa,startwith("dom2"))
f (int a, int b)
{
  _Bool _3;
  _Bool _4;
  _Bool _5;

  __BB(2):
  _3 = a_1(D) > b_2(D);
  _4 = b_2(D) > a_1(D);
  _5 = _3 | _4;
  return _5;
}

__attribute__((noipa))
_Bool __GIMPLE (ssa,startwith("dom2"))
f1 (int a, int b)
{
  _Bool _3;
  _Bool _4;
  _Bool _5;

  __BB(2):
  _3 = a_1(D) > b_2(D);
  _4 = a_1(D) < b_2(D);
  _5 = _3 | _4;
  return _5;
}

int main()
{
  int a = 10, b = 10;
  if (f(a,b))
    __builtin_abort();
  if (f1(a,b))
    __builtin_abort();
  return 0;
}

/* { dg-final { scan-tree-dump-not "return 1" "optimized" } } */
/* Both functions should be optimized to `a_1 != b_2`. */
/* { dg-final { scan-tree-dump-times "a_1.D. != b_2.D." 2 "optimized" { target __OPTIMIZE__ } } } */
