/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-sccp -fdump-tree-optimized" } */

int
const_init_unused_phi (void)
{
  int x = 0;
  for (int i = 0, y = 666; i != 19; i++, y = i * 10)
    x = y;
  return x;
}

int
conditional_init_unused_phi (int c)
{
  int x = 0;
  int y;

  if (c != 0)
    y = 20;
  else
    y = 30;

  for (int i = 0; i != 19; i++, y = i * 10)
    x = y;

  return x;
}

int
nested_unused_phi (void)
{
  int acc = 0;

  for (int i = 0; i < 20; i++)
    {
      int x = 0;
      for (int j = 0, y = 123; j != 19; j++, y = i + 10)
	x = y;
      acc += x;
    }

  return acc;
}

/* Expected:
   const_init_unused_phi         -> 180
   conditional_init_unused_phi   -> 180
   nested_unused_phi             -> sum_{i=0}^{19} (i + 10) = 390
   There should be no loop left in the optimized dump.
*/

/* { dg-final { scan-tree-dump-times "return 180" 2 "sccp" } } */
/* { dg-final { scan-tree-dump "return 390" "sccp" } } */
/* { dg-final { scan-tree-dump-times "goto" 0 "optimized" } } */
