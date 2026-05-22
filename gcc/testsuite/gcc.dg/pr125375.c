/* { dg-do run } */
/* { dg-options "-O2 -fno-inline -fno-compare-elim" } */

int a, c = 1;
short b = 1, e;
volatile unsigned short d;
static int f() {
  unsigned g = 2;
  if (c >= a)
    if ((c || b) && b) {
      unsigned h = c && d;
      int i = e = d;
      if (d)
        i = d = 0;
      g = c ^ g * i;
      c = ~c;
      b = b * (g | 9) & ((1 && a) - i);
      h && i && d;
      a = c ^ e ^ (g && a) * h;
      d = e;
      if (a)
        return g;
    }
}
int main() {
  if (f() != 1)
    __builtin_abort();
  return 0;
}
