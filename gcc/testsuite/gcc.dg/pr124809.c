/* { dg-do run } */
/* { dg-options "-O1 -fno-tree-ch -fno-tree-dce" } */
unsigned short a, c, d;
short f(short g, short h) { return h; }
short f1(short g, short h) { return g; }
__attribute__((noipa)) void sink(...){__builtin_abort();}
__attribute__((noinline))
int i(signed char g) {
  unsigned short j;
  int b=0, k;
  while (d < 8) {
    if ((b = a != g) ^ a) {
      j = b + a;
      k = j;
      return a ? 0 : k >> a;
    }
   b = f(d && c, a);
  }
  sink(b);
  return 100;
}
__attribute__((noinline))
int i1(signed char g) {
  unsigned short j;
  int b=0, k;
  while (d < 8) {
    if ((b = a != g) ^ a) {
      j = b + a;
      k = j;
      return a ? 0 : k >> a;
    }
   b = f1(a, d && c);
  }
  sink(b);
  return 100;
}
int main() {
  int t = i(1);
  if (t != 1)
    __builtin_abort();
  t = i1(1);
  if (t != 1)
    __builtin_abort();
  return 0;
}
