/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx -O2" } */
/* { dg-require-effective-target powerpc_vsx } */

double load_asm_d_constraint (signed char *p)
{
  double ret;
  __asm__ ("xxlor %x0,%x1,%x1\t# load d constraint" : "=d" (ret) : "d" (*p));
  return ret;
}

void store_asm_d_constraint (signed char *p, double x)
{
  signed char i;
  __asm__ ("xxlor %x0,%x1,%x1\t# store d constraint" : "=d" (i) : "d" (x));
  *p = i;
}

/* { dg-final { scan-assembler "lxsibzx" } } */
/* { dg-final { scan-assembler "stxsibx" } } */
