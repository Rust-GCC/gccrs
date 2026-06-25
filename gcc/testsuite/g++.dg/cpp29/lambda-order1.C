// P3847R1 - Lexical order for lambdas
// { dg-do run { target c++14 } }

int e;

int
foo (int x)
{
  if (x != e)
    __builtin_abort ();
  ++e;
  return x;
}

int &
bar (int x)
{
  if (x != e)
    __builtin_abort ();
  ++e;
  return e;
}

int
main ()
{
  int a = 1;
  long b = 2;
  double c = 3;
  short d = 4;
  foo (0);
  auto f = [&, a0 = foo (1), a, a1 = foo (2),
	    b0 = foo (3), b, b1 = foo (4),
	    c0 = foo (5), c, c1 = foo (6)] () {
    if (&a0 >= &a
	|| &a >= &a1
	|| (const void *) &a1 >= (const void *) &b0
	|| (const void *) &b0 >= (const void *) &b
	/* d is captured implicitly, the ordering for
	   the d member vs. the explicitly captured
	   ones is still unspecified by C++, but part
	   of the Itanium ABI.  Just test that it doesn't
	   change the order of the explicit captures.  */
	|| d != 4
	|| (const void *) &b >= (const void *) &b1
	|| (const void *) &b1 >= (const void *) &c0
	|| (const void *) &c0 >= (const void *) &c
	|| (const void *) &c >= (const void *) &c1)
      __builtin_abort ();
  };
  foo (7);
  f ();
  foo (8);
  f ();
  foo (9);
  auto g = [=, &a0 = bar (10), &a, &a1 = bar (11),
	    &b0 = bar (12), &b, &b1 = bar (13),
	    &c0 = bar (14), &c, &c1 = bar (15)] () {
    return a0 + a + a1 + b0 + b + b1 + c0 + c + c1;
  };
  foo (16);
  g ();
  foo (17);
  auto h = [&, a0 = foo (18), a, a1 = foo (19),
	    b0 = foo (20), b, b1 = foo (21),
	    c0 = foo (22), c, c1 = foo (23)] () {
    if (&a0 >= &a
	|| &a >= &a1
	|| (const void *) &a1 >= (const void *) &b0
	|| (const void *) &b0 >= (const void *) &b
	|| (const void *) &b >= (const void *) &b1
	|| (const void *) &b1 >= (const void *) &c0
	|| (const void *) &c0 >= (const void *) &c
	|| (const void *) &c >= (const void *) &c1)
      __builtin_abort ();
  };
  foo (24);
  h ();
  foo (25);
}
