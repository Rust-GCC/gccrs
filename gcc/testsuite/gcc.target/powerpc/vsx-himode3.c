/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx -O2" } */
/* { dg-require-effective-target powerpc_vsx } */

double load_asm_v_constraint (short *p)
{
  double ret;
  __asm__ ("xxlor %x0,%x1,%x1\t# load v constraint" : "=d" (ret) : "v" (*p));
  return ret;
}

void store_asm_v_constraint (short *p, double x)
{
  short i;
  __asm__ ("xxlor %x0,%x1,%x1\t# store v constraint" : "=v" (i) : "d" (x));
  *p = i;
}

/* { dg-final { scan-assembler "lxsihzx" } } */
/* { dg-final { scan-assembler "stxsihx" } } */
