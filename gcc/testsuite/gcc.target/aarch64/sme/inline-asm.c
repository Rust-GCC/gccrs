/* This test checks the use of the Uc[ij] constraints in inline asm.  */
/* { dg-do compile } */
/* { dg-options "-O2" } */

void ucj(unsigned v, void *p)
{
  asm volatile ("str za[%w0, #0], [%1]" :: "Ucj"(v), "r"(p));
}

void uci(unsigned wv)
{
  asm volatile ("mova { z0.d-z1.d }, za.d[%w0, #0, vgx2]" :: "Uci"(wv));
}

/* { dg-final { scan-assembler-times {str za\[w1[2-5], #0\], \[x1\]} 1 } } */
/* { dg-final { scan-assembler-times {mova { z0\.d-z1\.d }, za\.d\[w([89]|1[01]), #0, vgx2\]} 1 } } */
