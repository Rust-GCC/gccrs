/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "and\[lq\]?\[\\t \]*\\$-32,\[\\t \]*%\[re\]?sp" { xfail ia32 } } } */

typedef struct
{
  long double a;
  long double b;
} c __attribute__((aligned(32)));
extern double d;
void
bar (c f)
{
  d = f.a;
}
