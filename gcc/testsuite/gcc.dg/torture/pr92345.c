/* { dg-do compile } */
/* { dg-additional-options "-ftree-vectorize" } */

long int x1;
int fr;

int
us (int sk, int jx)
{
  while (sk < 1)
    {
      jx *= 2;
      fr += x1 + 1;
      ++sk;
    }

  return jx;
}
