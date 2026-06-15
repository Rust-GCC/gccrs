/* { dg-do compile } */
/* { dg-options "-O2 -fno-dce -fno-tree-fre -march=x86-64" } */

int b, c;
int
a (void)
{
  char d[8];
  int e = 0;
  if (b)
    goto f;
  if (c)
    {
      for (int g = 0; g < 8; ++g)
        d[g] = g;
      a ();
      d[0] = -d[0];
      d[1] = -d[1];
    f:
      int h;
      if (h)
        __builtin_abort ();
      e = d[0];
      e = d[1] + e;
    }
  return e;
}
