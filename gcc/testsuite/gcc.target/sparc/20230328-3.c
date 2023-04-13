/* PR target/109140 */
/* { dg-do compile } */
/* { dg-options "-O3 -mvis4 -std=c99" } */

#define TYPE unsigned short

struct S { TYPE ub[4]; };

struct S s;

TYPE v;

void foo (void)
{
  for (int i = 0; i < 4; i++)
    s.ub[i] = s.ub[i] > v;
}

/* { dg-final { scan-assembler "fpcmpugt16\t%" } } */
