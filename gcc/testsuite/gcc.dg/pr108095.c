/* PR tree-optimization/108095 */
/* { dg-do compile } */
/* { dg-options "-Os -g" } */

int v;
typedef unsigned T;

void
foo (void)
{
  unsigned s;
  asm goto ("" : "=r" (*(T *) &s) : : : lab);
  v = 0;
lab:
}
