/* PR rtl-optimization/16643 */
/* { dg-skip-if "too many arguments in function call" { bpf-*-* } } */

void foo (int a, int b, int c, int d, int e, int *f)
{
  if (a == 0)
    if (b == 0)
      if (c == 0)
	if (d == 0)
	  {
	    *f = e;
	    return;
	  }
  *f = e;
  return;
}
