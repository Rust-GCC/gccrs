/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=v4" } */

int bar_int(void);
short bar_short(void);
_Bool bar_bool(void);

int foo_int(void) {
      if (bar_int() != 1) return 0; else return 1;
}

/* { dg-final { scan-assembler-not {r0 = r0} } } */
/* { dg-final { scan-assembler-times {w0 = w0} 1 } } */

int trigger_zext_hidi(void) {
      if (bar_short() != 1) return 0; else return 1;
}

/* { dg-final { scan-assembler-not {r0 &= 0xffff} } } */
/* { dg-final { scan-assembler-times {w0 &= 0xffff} 1 } } */

int trigger_zext_qidi(void) {
      if (bar_bool() != 0) return 0; else return 1;
}

/* { dg-final { scan-assembler-not {r0 &= 0xff\n} } } */
/* { dg-final { scan-assembler-times {w0 &= 0xff\n} 1 } } */
