/* PR c/125629 */
/* { dg-do compile } */

int c;

unsigned
foo ()
{
  return __builtin_bswapg (c ? : 0u);
}
