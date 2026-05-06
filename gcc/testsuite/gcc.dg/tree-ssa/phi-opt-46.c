/* PR tree-optimization/110262 */
/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiopt2" } */

int g(int min_need_stall)
{
  int t = ((min_need_stall) < (1) ? (min_need_stall) : (1));
  return  min_need_stall < 0 ? 1 : t;
}

/* This should be converted into (int)MIN<(unsigned)min_need_stall, 1u> */

/* { dg-final { scan-tree-dump "MIN_EXPR <"  "phiopt2" } } */
/* { dg-final { scan-tree-dump-not "if "  "phiopt2" } } */

