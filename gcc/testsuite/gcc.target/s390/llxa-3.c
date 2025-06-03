/* { dg-do compile { target { lp64 } } } */
/* { dg-options "-O1 -march=arch15" } */
/* { dg-final { scan-assembler {\tllxah\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */
/* { dg-final { scan-assembler {\tllxaf\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */
/* { dg-final { scan-assembler {\tllxag\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */
/* { dg-final { scan-assembler {\tllxaq\t%r[0-9]+,42\(%r[0-9]+,%r[0-9]+\)} } } */

char *
llxab (char *a, unsigned long j)
{
  unsigned int i = (unsigned int)j + 42;
  return &a[i];
}

short *
llxah (short *a, unsigned long j)
{
  unsigned int i = (unsigned int)j + 42;
  return &a[i];
}

int *
llxaf (int *a, unsigned long j)
{
  unsigned int i = (unsigned int)j + 42;
  return &a[i];
}

long long *
llxag (long long *a, unsigned long j)
{
  unsigned int i = (unsigned int)j + 42;
  return &a[i];
}

__int128 *
llxaq (__int128 *a, unsigned long j)
{
  unsigned int i = (unsigned int)j + 42;
  return &a[i];
}
