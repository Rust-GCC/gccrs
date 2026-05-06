/* { dg-do compile } */
/* { dg-additional-options "-O" } */

int foo (char c, int i)
{
  int s = 0;
  while (i--)
    s += c;
  return s;
}

