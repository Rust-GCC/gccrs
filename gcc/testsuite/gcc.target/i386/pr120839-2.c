/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "and\[lq\]?\[\\t \]*\\$-32,\[\\t \]*%\[re\]?sp" } } */

typedef struct
{
  long double a;
  long double b;
} c __attribute__((aligned(32)));
extern c x;
extern double d;
extern void bar (c);
void
foo (void)
{
  x.a = d;
  x.b = d;
  bar (x);
}
