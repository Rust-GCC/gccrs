/* PR c/125629 */
/* { dg-do compile } */

int c;

unsigned
foo ()
{
  return __builtin_bitreverseg (c ? : 0u);
}
