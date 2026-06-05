/* Test __builtin_bitreverseg.  */
/* { dg-do compile } */
/* { dg-options "" } */

int
main ()
{
  __builtin_bitreverseg ();		/* { dg-error "wrong number of arguments to '__builtin_bitreverseg'" } */
  __builtin_bitreverseg (1U, 1U);	/* { dg-error "wrong number of arguments to '__builtin_bitreverseg'" } */
  __builtin_bitreverseg (1.0);		/* { dg-error "'__builtin_bitreverseg' operand not an integral type" } */
  __builtin_bitreverseg (1.0f);		/* { dg-error "'__builtin_bitreverseg' operand not an integral type" } */
  __builtin_bitreverseg (1.0L);		/* { dg-error "'__builtin_bitreverseg' operand not an integral type" } */
  struct S { int a; };
  __builtin_bitreverseg ((struct S) { 42 });	/* { dg-error "'__builtin_bitreverseg' operand not an integral type" } */
  enum E { E0, E1 } e;
  __builtin_bitreverseg (E1);		/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has signed type" "" { target c } } */
					/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has enumerated type" "" { target c++ } .-1 } */
  __builtin_bitreverseg (e);		/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has enumerated type" } */
  __builtin_bitreverseg ((enum E) 1);	/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has enumerated type" } */
#if !defined(__cplusplus) || __cplusplus >=  201103L
  enum F : int { F0, F1 } f;
  __builtin_bitreverseg (F1);		/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has enumerated type" "" { target { c || c++11 } } } */
  __builtin_bitreverseg (f);		/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has enumerated type" "" { target { c || c++11 } } } */
  __builtin_bitreverseg ((enum F) 1);	/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has enumerated type" "" { target { c || c++11 } } } */
#endif
  __builtin_bitreverseg (false);	/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has boolean type" } */
  __builtin_bitreverseg (1);		/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has signed type" } */
  __builtin_bitreverseg ((char) 1);	/* { dg-error "argument 1 in call to function '__builtin_bitreverseg' has signed type" "" { target { ! unsigned_char } } } */
}
