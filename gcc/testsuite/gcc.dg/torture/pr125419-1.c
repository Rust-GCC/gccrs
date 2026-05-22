/* { dg-do compile } */
/* PR tree-optimization/125419 */

signed  g13, g29, g27, g26, g17;
void f2(void)__attribute__((returns_twice));
void f1(bool c14)
{
  int sj15;
  if (c14) sj15 = sj15;
  f2();
  c14 = 0;
  do {
    int c11 = g27;
    if (c11) goto lbl_br22;
    goto lbl_br24;
lbl_br22:
    c14 = sj15;
    c11 = g29 = g13;
    if (c11) goto lbl_br22;
lbl_br24:
    g26 = g17;
  } while (c14);
}
