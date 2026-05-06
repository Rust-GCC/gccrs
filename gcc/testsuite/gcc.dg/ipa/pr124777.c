/* { dg-do run } */
/* { dg-options "-O2 -fno-early-inlining" } */

int a;
char b;
void c() {
    __builtin_exit(0);
}

typedef short (*t_d)(unsigned);

static short __attribute__((noclone))
d(unsigned f)
{
  if (f == (unsigned)(unsigned short)-1)
    c ();
  for (; a; a++)
    ;
  return 0;
}

typedef short (*t_g)(unsigned, unsigned, unsigned short, t_d);

static short  __attribute__((noclone))
g(unsigned f, unsigned h, unsigned short i, t_d p_d) {
  if (f || h)
    p_d(i);
  return 0;
}

static unsigned  __attribute__((noclone))
j(unsigned f, signed char h, t_d p_d, t_g p_g) {
  p_g(b, 1, h, p_d);
  return 0;
}

int main() {
  signed char e = -1;
  j(0, e, d, g);
  return 0;
}
