/* { dg-do compile { target int128 } } */
/* { dg-options "-march=z16 -O3" } */

int a, b, c;
long d, e;
int *f;
__int128 g;
void h() {
  g = 0;
  for (; g <= 33; ++g) {
    int *i = &c;
    *i = d - a;
    if (*i) {
      c = 0;
      for (; c <= 8; ++c)
        f = &b;
    } else
      ++e;
  }
}
