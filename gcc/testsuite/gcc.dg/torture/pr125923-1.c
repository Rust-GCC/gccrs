/* { dg-do compile } */
/* { dg-additional-options "-fno-tree-dce" } */
/* PR tree-optimization/125923 */

int a, b, c, d;
void e() {
  d = a && d;
  if (b)
    b = 0;
  while (!a || b)
    c ^= 1;
}
