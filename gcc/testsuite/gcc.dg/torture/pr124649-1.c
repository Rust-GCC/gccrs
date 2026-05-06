/* PR rtl-optimization/124649 */
/* { dg-do compile } */
/* { dg-additional-options "-mcpu=neoverse-v1" { target aarch64*-*-* } } */

void f(char *g, int b) {
  int k = b & 4 + 1;
  int c[b];
  if (k == 1)
    b >>= 3;
  b &= 1;
  c[0] = b;
  for (int i = 0; i < k; i++)
    g[i] = c[i];
}

