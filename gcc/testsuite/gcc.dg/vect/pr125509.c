/* { dg-add-options vect_early_break } */
/* { dg-do compile } */
/* { dg-require-effective-target vect_early_break } */
/* { dg-require-effective-target vect_int } */
/* { dg-additional-options "-mavx512f" { target { i?86-*-* x86_64-*-* } } } */

struct a {
  char *b;
  long c;
} e;
long d;
char f;
static bool g(char *h, long j) {
  auto i = h + j - 1;
  for (; h != i; h++)
    if (*h)
      return false;
  return f;
}
void k();
static void l(struct a h) {
  if (!g(h.b, h.c))
    if (d)
      k();
}
void m() {
  if (e.c != 1)
    l(e);
}

/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" } } */
/* { dg-final { scan-tree-dump "early break does not require epilog" "vect" } } */
