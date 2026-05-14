/* { dg-do compile { target { musttail && { c || c++11 } } } } */

extern int f(void);

static int g(void) {
  [[gnu::musttail]] return f();
  return 42;
}

static int g2(void) {
  if (1) [[gnu::musttail]] return f();
  return 42;
}
