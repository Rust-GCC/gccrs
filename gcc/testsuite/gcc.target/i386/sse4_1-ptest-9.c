/* PR target/90483 */
/* PR libstdc++/118416 */
/* { dg-do compile } */
/* { dg-options "-O2 -msse4.1" } */

typedef long long v2di __attribute__ ((__vector_size__ (16)));

v2di x;

int test1 ()
{
  return __builtin_ia32_ptestz128((v2di){0,0}, x == 0);
}

int test2 ()
{
  return __builtin_ia32_ptestz128(x == 0, (v2di){0,0});
}

/* { dg-final { scan-assembler-not "ptest\[ \\t\]+%" } } */
/* { dg-final { scan-assembler-not "pcmpeq" } } */
