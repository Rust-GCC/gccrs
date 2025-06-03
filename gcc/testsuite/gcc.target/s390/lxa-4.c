/* { dg-do compile { target { lp64 } } } */
/* { dg-options "-O1 -march=arch15" } */
/* { dg-final { scan-assembler {\tlxab\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */
/* { dg-final { scan-assembler {\tlxah\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */
/* { dg-final { scan-assembler {\tlxaf\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */
/* { dg-final { scan-assembler {\tlxag\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */
/* { dg-final { scan-assembler {\tlxaq\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */

char *
lxab (char *a, long j)
{
  int i = (int)j + 42;
  return &a[i];
}

short *
lxah (short *a, long j)
{
  int i = (int)j + 42;
  return &a[i];
}

int *
lxaf (int *a, long j)
{
  int i = (int)j + 42;
  return &a[i];
}

long long *
lxag (long long *a, long j)
{
  int i = (int)j + 42;
  return &a[i];
}

__int128 *
lxaq (__int128 *a, long j)
{
  int i = (int)j + 42;
  return &a[i];
}
