/* { dg-do compile } */
/* { dg-additional-options "-fcompare-debug" } */
/* PR tree-optimization/125776 */

int a, b, d, e, f;
long c;
void g();
void h() {
  unsigned i;
  for (;;) {
    unsigned j = a ? a : d;
    i += j;
    f = b & 40 | b > 4;
    e = a ? a : b;
    if (e)
      break;
    if (j)
      g();
  }
  c = i;
}
