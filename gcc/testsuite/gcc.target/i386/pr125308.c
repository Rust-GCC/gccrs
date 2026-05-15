/* PR target/125308 */
/* { dg-do compile } */
/* { dg-options "-msse2 -O2 -ffloat-store" } */

float a[4];

void
foo ()
{
  for (int i = 0; i < 4; i++)
    a[i] = a[i] > 0 ? a[i] : 0;
}
