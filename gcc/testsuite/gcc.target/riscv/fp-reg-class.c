/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-O3" "-Og" "-Os" "-Oz" "-flto" } } */
/* { dg-options "-march=rv64gc -mabi=lp64d -O2 -fdump-rtl-ira" } */

/* When riscv_regno_to_class[] mapped every FP hard register to
   RVC_FP_REGS, ira-costs.cc:setup_allocno_cost_vector read the cost
   slot for f0-f7 / f16-f31 from the wrong bucket and IRA mis-allocated
   FP pseudos.  Eight "cf"- and sixteen "f"-constrained doubles live
   across a call expose this: in the buggy state IRA picks an
   all-in-hardreg coloring with no spills ("+++Costs" shows "mem 0")
   and LRA recovers with fmv.d to fs* registers; with the fix IRA
   spills the cf values honestly and the "mem" component is non-zero.  */

extern void use (double, double, double, double,
		 double, double, double, double);

double
test (double *p, int n)
{
  double f0 = p[0], f1 = p[1], f2 = p[2], f3 = p[3];
  double f4 = p[4], f5 = p[5], f6 = p[6], f7 = p[7];
  double f8 = p[8], f9 = p[9], f10 = p[10], f11 = p[11];
  double f12 = p[12], f13 = p[13], f14 = p[14], f15 = p[15];

  double c0, c1, c2, c3, c4, c5, c6, c7;
  asm ("fadd.d %0,%1,%1" : "=cf" (c0) : "cf" (p[16]));
  asm ("fadd.d %0,%1,%1" : "=cf" (c1) : "cf" (p[17]));
  asm ("fadd.d %0,%1,%1" : "=cf" (c2) : "cf" (p[18]));
  asm ("fadd.d %0,%1,%1" : "=cf" (c3) : "cf" (p[19]));
  asm ("fadd.d %0,%1,%1" : "=cf" (c4) : "cf" (p[20]));
  asm ("fadd.d %0,%1,%1" : "=cf" (c5) : "cf" (p[21]));
  asm ("fadd.d %0,%1,%1" : "=cf" (c6) : "cf" (p[22]));
  asm ("fadd.d %0,%1,%1" : "=cf" (c7) : "cf" (p[23]));

  for (int i = 0; i < n; ++i)
    {
      f0  = f0  * f1  + f2;   f1  = f1  * f2  + f3;
      f2  = f2  * f3  + f4;   f3  = f3  * f4  + f5;
      f4  = f4  * f5  + f6;   f5  = f5  * f6  + f7;
      f6  = f6  * f7  + f8;   f7  = f7  * f8  + f9;
      f8  = f8  * f9  + f10;  f9  = f9  * f10 + f11;
      f10 = f10 * f11 + f12;  f11 = f11 * f12 + f13;
      f12 = f12 * f13 + f14;  f13 = f13 * f14 + f15;
      f14 = f14 * f15 + f0;   f15 = f15 * f0  + f1;
      asm ("fadd.d %0,%0,%1" : "+cf" (c0) : "f" (f0));
      asm ("fadd.d %0,%0,%1" : "+cf" (c1) : "f" (f1));
      asm ("fadd.d %0,%0,%1" : "+cf" (c2) : "f" (f2));
      asm ("fadd.d %0,%0,%1" : "+cf" (c3) : "f" (f3));
      asm ("fadd.d %0,%0,%1" : "+cf" (c4) : "f" (f4));
      asm ("fadd.d %0,%0,%1" : "+cf" (c5) : "f" (f5));
      asm ("fadd.d %0,%0,%1" : "+cf" (c6) : "f" (f6));
      asm ("fadd.d %0,%0,%1" : "+cf" (c7) : "f" (f7));
      use (f8, f9, f10, f11, f12, f13, f14, f15);
    }
  return f0+f1+f2+f3+f4+f5+f6+f7+f8+f9+f10+f11+f12+f13+f14+f15
       + c0+c1+c2+c3+c4+c5+c6+c7;
}

/* { dg-final { scan-rtl-dump {\+\+\+Costs:[^\n]* mem [1-9]} "ira" } } */
