/* Test __builtin_bswapg.  */
/* { dg-do compile } */
/* { dg-options "" } */

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 575
  __builtin_bswapg (0x0uwb);	/* { dg-error "precision 1 of argument 1 to function '__builtin_bswapg' is not a multiple of 8" "" { target bitint575 } } */
  __builtin_bswapg (0x3uwb);	/* { dg-error "precision 2 of argument 1 to function '__builtin_bswapg' is not a multiple of 8" "" { target bitint575 } } */
  __builtin_bswapg (0x34938571398572uwb); /* { dg-error "precision 54 of argument 1 to function '__builtin_bswapg' is not a multiple of 8" "" { target bitint575 } } */
  __builtin_bswapg (0x34938571398572abcd9837298dabef9187983uwb); /* { dg-error "precision 146 of argument 1 to function '__builtin_bswapg' is not a multiple of 8" "" { target bitint575 } } */
  __builtin_bswapg (0x34938571398572abcd9837298dabef918798329387987abef98792837549857983745983475329847uwb); /* { dg-error "precision 322 of argument 1 to function '__builtin_bswapg' is not a multiple of 8" "" { target bitint575 } } */
#endif
}
