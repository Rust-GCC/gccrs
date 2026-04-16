/* PR tree-optimization/124891 */
/* { dg-do compile } */

void
foo (int x, int y, int z, int *buf)
{
  for (int i = 0; i < x; ++i)
    for (int j = z; j < x-z; ++z)
      buf[j + (y - z +i) * x] = buf[x];
}
