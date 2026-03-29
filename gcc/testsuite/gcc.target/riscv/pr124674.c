/* { dg-do compile { target { rv64 } } } */
/* { dg-additional-options "-Wno-pedantic -march=rv64gc -mabi=lp64d" } */

struct {
  __int128 a;
  int c[64];
} s;

int g;

void foo() { __builtin_mul_overflow(*(_Complex long *)&s.c[1] ? 1 : 0, 1, &g); }
