/* { dg-do compile } */
/* { dg-additional-options "-fno-tree-fre -fno-tree-pre" } */

char c;
int a, b;

void foo (int j)
{
  int i;
  while (--j)
    {
      b = 3;
      for (i = 0; i < 2; ++i)
        a = b ^ c;
    }
}

