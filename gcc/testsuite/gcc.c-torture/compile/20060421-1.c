/* This test used to ICE on ARM with -mcpu=iwmmxt.  */
/* { dg-require-stack-size "249*8+1" } */

void
foo (void)
{
  long long int a;
  unsigned long b[249]; /* >= 249 causes failure */
  register unsigned int c;
  b[c] = (a & (1ULL << c)) ? 1 : 0;
}
