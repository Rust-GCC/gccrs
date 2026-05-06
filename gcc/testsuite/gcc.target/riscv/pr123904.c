/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" } } */

unsigned foo19(unsigned a, unsigned b) { b = (b << 2) >> 2; return a + (b << 1); }

/* { dg-final { scan-assembler-times "slli" 1 } } */
/* { dg-final { scan-assembler-times "srli" 1 } } */
/* { dg-final { scan-assembler-times "add" 1 } } */
