/* { dg-do compile } */
/* { dg-options "-fdump-tree-lower" } */

/* Basic MRE from bug report */
void test_bare (void *d, const void *s, __SIZE_TYPE__ n) {
  __builtin_mempcpy (d, s, n);
}

/* { dg-final { scan-tree-dump "__builtin_memcpy" "lower" } } */
