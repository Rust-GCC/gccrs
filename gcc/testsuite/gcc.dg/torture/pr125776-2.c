/* { dg-do compile } */
/* { dg-additional-options "-fcompare-debug" } */
/* PR tree-optimization/125776 */
int a, b, d, e, f;
long c;
void g();
void h(int i) {
  for (;;) {
    i++;
    int j = a  ? a : d;
    i = i > 0 ? i : -i;
    f = b & 40 | b > 4;
    e = a ? a : b;
    if (e)
      break;
    if (j)
      g();
  }
  c = i;
}

