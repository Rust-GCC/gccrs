/* PR tree-optimization/110262 */
/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-gimple" } */

int g(int min_need_stall)
{
  return  min_need_stall < 0 ? 1 : ((min_need_stall) < (1) ? (min_need_stall) : (1));
}

/* { dg-final { scan-tree-dump "MIN_EXPR <"  "gimple" } } */
/* { dg-final { scan-tree-dump-not "if "  "gimple" } } */

