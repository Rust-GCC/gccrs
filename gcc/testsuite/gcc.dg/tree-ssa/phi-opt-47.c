/* PR tree-optimization/110262 */
/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiopt2"  } */
int g(int a, int b)
{
  b = b < 0 ? 0 : b;
  int t = ((a) < (b) ? (a) : (b));
  return a < 0 ? b : t;
}

int g1(int a, int b, unsigned short c)
{
  b = c;
  int t = ((a) < (b) ? (a) : (b));
  return a < 0 ? b : t;
}

/* Both of the last statement of these functions should be converted into:
   (int)MIN<(unsigned)a, (unsigned)b>.  */

/* { dg-final { scan-tree-dump "MIN_EXPR <"  "phiopt2" } } */
/* { dg-final { scan-tree-dump-not "if "  "phiopt2" } } */
/* { dg-final { scan-tree-dump-times "Removing basic block" 2  "phiopt2" } } */

