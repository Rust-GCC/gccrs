/* { dg-do compile } */
/* { dg-options "-O3 -march=x86-64" } */

int a, b;
int c[0], d[0];
void
foo (void)
{
  __asm__ goto("" : "=r"(a) : : : f);
f:
  for (; b; b++)
    c[b] = d[b] = c[b] + a;
}
