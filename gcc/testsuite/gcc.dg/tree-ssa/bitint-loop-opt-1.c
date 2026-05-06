/* PR tree-optimization/124061 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -fdump-tree-sccp" } */

int f(_BitInt(64) limit)
{
  _BitInt(64) i;
  for(i = 0; i < limit; i++)
   ;
  return i;
}

/* { dg-final { scan-tree-dump "final value replacement:"  sccp } } */
