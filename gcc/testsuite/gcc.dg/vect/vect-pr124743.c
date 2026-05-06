/* { dg-do compile } */
/* { dg-additional-options "-O3 -fno-tree-loop-distribute-patterns -fno-tree-loop-distribution -fno-tree-ccp -fno-tree-copy-prop -fno-tree-dse" } */

int a, b, c, d;
int e(int f, int g) { return g < 0 || g > 1 ? 0 : f >> g; }
int h(int i) { return a > 1 ? 0 : i << a; }
int main()
{
  for (; b; b++)
    c = e(h(1), d);
  return 0;
}
