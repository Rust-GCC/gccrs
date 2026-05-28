/* PR target/125469 */
/* { dg-do compile { target int128 } } */
/* { dg-options "-Ofast -mapx-features=nf" } */

int s;

void
foo ()
{
  s /= ((__int128) 0x33333333333333333333333333333333uwb) + 1i;
}
