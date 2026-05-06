/* PR rtl-optimization/116600 */

int a, b;
int foo ();

int
bar ()
{
  int c = ({ int d = 0, e = foo ();
	     b = __builtin_expect (e, 1); if (b) ; else d = 4; d; });
  if (c)
    return c;
  foo ();
  if (a)
    __asm__ goto ("" : : : : l);
l:
  return 0;
}

void
baz ()
{
  bar ();
}
