/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-gimple-raw -fdump-tree-forwprop1-raw" } */
/* PR tree-optimization/125104 */

int f(int a, int b)
{
  a &= ~1;
  a ^= 1;
  return a; // a | 1
}

/* { dg-final { scan-tree-dump "bit_xor_expr" "gimple" } } */
/* { dg-final { scan-tree-dump "bit_ior_expr" "forwprop1" } } */
