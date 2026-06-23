/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

_Bool
a_mod_b_lt_b (int a, int b) {
  return ((a % b) < b) == (b >= 0);
}

_Bool
a_mod_b_ge_b (int a, int b) {
  return ((a % b) >= b) == (b < 0);
}

/* { dg-final { scan-tree-dump-times "return 1;" 2 "optimized" } } */
