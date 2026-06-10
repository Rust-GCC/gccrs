/* PR c/125694 */
/* { dg-do compile } */

void
foo ()
{
  __builtin_bitreverseg (__builtin_bitreverseg ());	/* { dg-error "wrong number of arguments to '__builtin_bitreverseg'" } */
}
