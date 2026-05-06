/* { dg-do compile } */
/* { dg-additional-options "-O3 -fno-tree-copy-prop" } */

int printf(const char *, ...);
int a, b, c, d, e;
int g(int h) { return h >> a; }
int main()
{
  int j = c;
  while (a)
    while (1)
      {
	while (1)
	  {
	    for (e = 0; e < 16; e++)
	      {
		d = b;
		j && (b = g(2));
	      }
	    break;
	  }
	printf("%d\n", b);
      }
  return 0;
}
