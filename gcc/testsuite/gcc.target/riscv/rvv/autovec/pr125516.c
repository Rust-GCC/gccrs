/* { dg-do run { target riscv_v } } */
/* { dg-additional-options "-fdump-tree-optimized" } */

char a;
long long b;
short d[18][18][18][18];
short (*e)[18][18][18] = d;

int main ()
{
  for (long h = 0; h < 4; ++h)
    for (long i = 0; i < 18; ++i)
      for (long j = 0; j < 18; ++j)
	for (long k = 0; k < 18; ++k)
	  d[h][i][j][k] = j;

  for (short i = 0; i < 8; i++)
    for (unsigned char j = 0; j < 18; j++)
      a = e[3][i][i][j] ? e[j][i][i][3] : 0;

  b = (int) a;
  if (b != 0)
    __builtin_abort ();
}
