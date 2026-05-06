/* { dg-do compile } */

int a, b, d, e[3][1], f, g, h;
unsigned c;
int main()
{
  while (b)
    {
      h = g;
      for (b = 0; b < 2; b++)
	;
      for (; c; c++)
	{
	  int j = e[b][h];
	  f = a < 0 ? j : j - a;
	  if (!f)
	    break;
	  g = c;
	}
    }
  return 0;
}
