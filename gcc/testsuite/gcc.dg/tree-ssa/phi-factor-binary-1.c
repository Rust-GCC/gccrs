/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt1-details" } */

/* PR tree-optimization/125557.  Both arms of the diamond apply the same binary
   operation sharing one operand (* c), so phi-opt factors it out: the join
   becomes t = PHI <a, b>; x = t * c -- one multiply at the merge instead of
   one in each arm, and no extra PHI.  */

int
f (int cond, int a, int b, int c)
{
  int x;
  if (cond)
    x = a * c;
  else
    x = b * c;
  return x;
}

/* { dg-final { scan-tree-dump " changed to factor operation out from COND_EXP" "phiopt1" } } */
