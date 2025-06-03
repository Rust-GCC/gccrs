/* { dg-do compile } */
/* { dg-options "-w -O1 -fdump-tree-gimple" } */

typedef int SItype __attribute__ ((mode (SI)));
typedef SItype v4si __attribute__ ((vector_size (16)));

v4si vs (v4si a, v4si b)
{
  return __builtin_shuffle (a, b, (v4si) {0, 4, 1, 5});
}

/* The compound literal should be placed directly in the vec_perm.  */
/* { dg-final { scan-tree-dump-times "VEC_PERM_EXPR <a(?:\\.0)?, b(?:\\.1)?, { 0, 4, 1, 5 }>;" 1 "gimple" } } */

