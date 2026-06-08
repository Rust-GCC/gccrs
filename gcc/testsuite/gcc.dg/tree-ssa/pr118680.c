/* PR tree-optimization/118680 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

static inline uint64_t
max_u64 (uint64_t a, uint64_t b)
{
  return a > b ? a : b;
}

static inline uint64_t
min_u64 (uint64_t a, uint64_t b)
{
  return a < b ? a : b;
}

uint32_t
test_max (uint32_t a, uint32_t b)
{
  return max_u64 (a, b);
}

uint32_t
test_min (uint32_t a, uint32_t b)
{
  return min_u64 (a, b);
}

/* { dg-final { scan-tree-dump "MAX_EXPR <\[ab\]_\[0-9\]+\\(D\\), \[ab\]_\[0-9\]+\\(D\\)>" "optimized" } } */
/* { dg-final { scan-tree-dump "MIN_EXPR <\[ab\]_\[0-9\]+\\(D\\), \[ab\]_\[0-9\]+\\(D\\)>" "optimized" } } */
