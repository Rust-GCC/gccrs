/* PR target/125907 */
/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64 -mtune=generic" } */

/* Verify that "(a & (1 << b)) ? c : imm" and the equivalent
   "((a >> b) & 1) ? c : imm" forms use bt + cmov even when one of the
   cmov arms is an immediate.  The immediate arm is materialized into a
   register by the *cmov_bt<mode> splitter.  */

unsigned f0 (unsigned a, unsigned b, unsigned c)
{
  return (a & (1 << b)) ? c : 2;
}

unsigned f1 (unsigned a, unsigned b, unsigned c)
{
  return ((a >> b) & 1) ? c : 2;
}

unsigned f2 (unsigned a, unsigned b, unsigned c)
{
  return (a & (1 << b)) ? 2 : c;
}

unsigned f3 (unsigned a, unsigned b, unsigned c)
{
  return ((a >> b) & 1) ? 2 : c;
}

/* { dg-final { scan-assembler-times "bt\[l\]?\[ \\t\]+" 4 } } */
/* { dg-final { scan-assembler-times "cmov\[n\]*c" 4 } } */
/* { dg-final { scan-assembler-not "sal" } } */
/* { dg-final { scan-assembler-not "shr" } } */
