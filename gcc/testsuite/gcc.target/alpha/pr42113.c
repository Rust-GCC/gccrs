/* { dg-do compile } */
/* { dg-options "" } */

int foo (int a, int b)
{
  int bar = a * sizeof (int);

  if (b)
    bar += sizeof (int);

  return bar;
}
