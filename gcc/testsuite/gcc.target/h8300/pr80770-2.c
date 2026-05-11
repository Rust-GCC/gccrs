/* { dg-do compile } */
/* { dg-additional-options "-O2 -std=gnu99 -mint32" } */


int foop(int x) { x &= 0xf; x |= (x + 0x80); return x; }
int foox(int x) { x &= 0xf; x |= (x ^ 0x80); return x; }

/* { dg-final { scan-assembler-not "add" } } */
/* { dg-final { scan-assembler-not "xor" } } */
/* { dg-final { scan-assembler-times "and" 2 } } */
/* { dg-final { scan-assembler-times "or" 2 } } */
