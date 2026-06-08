/* { dg-do compile } */
/* { dg-additional-options "-fno-tree-fre -fno-tree-forwprop" } */

int a;
int main()
{
  int c = 1;
  while (1)
    {
      a = 2 * a;
      c = -1 - 1 / c;
      if (!c)
	break;
      a = -2 * a;
    }
  return 0;
}
