/* { dg-do compile } */

/* PR rtl-optimization/124062 */

void a(int);
double b;
void c(int d, int e) {
  int f = e;
  if (b < 0)
    f += d;
  a(f);
  a(e);
}

