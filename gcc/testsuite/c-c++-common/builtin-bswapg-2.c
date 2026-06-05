/* Test __builtin_bswapg.  */
/* { dg-do compile } */
/* { dg-options "" } */

int
main ()
{
  __builtin_bswapg ();		/* { dg-error "wrong number of arguments to '__builtin_bswapg'" } */
  __builtin_bswapg (1U, 1U);	/* { dg-error "wrong number of arguments to '__builtin_bswapg'" } */
  __builtin_bswapg (1.0);	/* { dg-error "'__builtin_bswapg' operand not an integral type" } */
  __builtin_bswapg (1.0f);	/* { dg-error "'__builtin_bswapg' operand not an integral type" } */
  __builtin_bswapg (1.0L);	/* { dg-error "'__builtin_bswapg' operand not an integral type" } */
  struct S { int a; };
  __builtin_bswapg ((struct S) { 42 });	/* { dg-error "'__builtin_bswapg' operand not an integral type" } */
  enum E { E0, E1 } e;
  __builtin_bswapg (E1);	/* { dg-error "argument 1 in call to function '__builtin_bswapg' has signed type" "" { target c } } */
				/* { dg-error "argument 1 in call to function '__builtin_bswapg' has enumerated type" "" { target c++ } .-1 } */
  __builtin_bswapg (e);		/* { dg-error "argument 1 in call to function '__builtin_bswapg' has enumerated type" } */
  __builtin_bswapg ((enum E) 1);/* { dg-error "argument 1 in call to function '__builtin_bswapg' has enumerated type" } */
#if !defined(__cplusplus) || __cplusplus >=  201103L
  enum F : int { F0, F1 } f;
  __builtin_bswapg (F1);	/* { dg-error "argument 1 in call to function '__builtin_bswapg' has enumerated type" "" { target { c || c++11 } } } */
  __builtin_bswapg (f);		/* { dg-error "argument 1 in call to function '__builtin_bswapg' has enumerated type" "" { target { c || c++11 } } } */
  __builtin_bswapg ((enum F) 1);/* { dg-error "argument 1 in call to function '__builtin_bswapg' has enumerated type" "" { target { c || c++11 } } } */
#endif
  __builtin_bswapg (false);	/* { dg-error "argument 1 in call to function '__builtin_bswapg' has boolean type" } */
  __builtin_bswapg (1);		/* { dg-error "argument 1 in call to function '__builtin_bswapg' has signed type" } */
  __builtin_bswapg ((char) 1);	/* { dg-error "argument 1 in call to function '__builtin_bswapg' has signed type" "" { target { ! unsigned_char } } } */
}
