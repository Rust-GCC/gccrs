/* { dg-add-options vect_early_break } */
/* { dg-do run } */
/* { dg-require-effective-target vect_early_break_hw } */
/* { dg-require-effective-target vect_long } */

/* { dg-additional-options "-mavx512f" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-require-effective-target avx512f_runtime { target i?86-*-* x86_64-*-* } } */

#include "tree-vect.h"

typedef struct {
  unsigned b;
  long long bits[2];
} c;
typedef struct {
  c *d;
} e, *bitmap;
e f;
int g;

__attribute__((noipa))
int h(bitmap i) {
  c j = *i->d;
  unsigned k, l = j.b < 4;
  for (; l < 2; l++) {
    long long m = j.bits[l];
    for (; k < 64; k++) {
      long long n = (long long)1LL << k;
      if (m & n)
        goto o;
    }
    k = 0;
  }
  return 0;
o:
  return 1;
}

int main() {

  check_vect ();

  c a;
  a.bits[0] = 1ull << 63;
  f.d = &a;
  g = h(&f);
  return g == 0;
}

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 1 "vect" { target { ! arm*-*-* } } } } */
/* { dg-final { scan-tree-dump "early break does not require epilog" "vect" { target { ! arm*-*-* } } } } */
