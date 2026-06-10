/* PR c/125694 */
/* { dg-do compile } */

void
foo ()
{
  __builtin_bswapg (__builtin_bswapg ());		/* { dg-error "wrong number of arguments to '__builtin_bswapg'" } */
}
