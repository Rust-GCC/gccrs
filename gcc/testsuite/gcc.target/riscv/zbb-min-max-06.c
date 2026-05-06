/* { dg-do compile } */
/* { dg-options "-march=rv64gc_zbb -mabi=lp64d -O2" } */
/* Verify that all four si3 min/max expansions set SUBREG_PROMOTED,
   so that no redundant sext.w is emitted after each operation.  */

int smin_si3(int a, int b) { return a < b ? a : b; }
int smax_si3(int a, int b) { return a > b ? a : b; }
unsigned umin_si3(unsigned a, unsigned b) { return a < b ? a : b; }
unsigned umax_si3(unsigned a, unsigned b) { return a > b ? a : b; }

/* { dg-final { scan-assembler-times {\mmin\t} 1 } } */
/* { dg-final { scan-assembler-times {\mmax\t} 1 } } */
/* { dg-final { scan-assembler-times {\mminu} 1 } } */
/* { dg-final { scan-assembler-times {\mmaxu} 1 } } */
/* { dg-final { scan-assembler-not {\msext\.w\M} } } */
