/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized" } */

/* Indirectly unused result */
void test_unused_indirect (void *d, const void *s, __SIZE_TYPE__ n) {
  void *a = __builtin_mempcpy (d, s, n);
  void *b = a;
}

/* Simple used result (in statement) */
void *test_used_simple (void *d, const void *s, __SIZE_TYPE__ n) {
  return __builtin_mempcpy (d, s, n);
}

/* More complicated used result (in expression) */
__SIZE_TYPE__ test_used_in_expr (char *d, const char *s, __SIZE_TYPE__ n) {
  return (char *)__builtin_mempcpy (d, s, n) - d;
}

/* Unused in all paths */
void *test_unused_indirect2 (void *d, const void *s, __SIZE_TYPE__ n) {
  void *a = __builtin_mempcpy (d, s, n);
  if (n > 20) {
	return (void *)20;
  }
  return (void *)7;
}

/* Used in at least one path */
void *test_maybe_used (void *d, const void *s, __SIZE_TYPE__ n) {
  void *a = __builtin_mempcpy (d, s, n);
  if (n > 20) {
    return a;
  }
  return (void *)0;
}

/* { dg-final { scan-tree-dump-times "__builtin_memcpy" 2 "optimized" } } */
/* { dg-final { scan-tree-dump-times "__builtin_mempcpy" 3 "optimized" } } */
