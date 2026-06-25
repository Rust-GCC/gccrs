// P3847R1 - Lexical order for lambdas
// { dg-do run { target c++11 } }

int
main ()
{
  int a = 1;
  long b = 2;
  double c = 3;
  short d = 4;
  auto f = [&, a, b, c] () {
    if ((const void *) &a >= (const void *) &b
	/* d is captured implicitly, the ordering for
	   the d member vs. the explicitly captured
	   ones is still unspecified by C++, but part
	   of the Itanium ABI.  Just test that it doesn't
	   change the order of the explicit captures.  */
	|| d != 4
	|| (const void *) &b >= (const void *) &c)
      __builtin_abort ();
  };
  f ();
  f ();
}
