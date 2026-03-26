/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=v4" } */

int bar_int(void);

int foo_int(void) {
      if (bar_int() != 1) return 0; else return 1;
}

/* { dg-final { scan-assembler-not {r0 = r0} } } */
/* { dg-final { scan-assembler-times {w0 = w0} 1 } } */
