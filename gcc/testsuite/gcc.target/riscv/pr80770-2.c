/* { dg-do compile } */
/* { dg-additional-options "-O2 -std=gnu99" } */


int foop(int x) { x &= 0xf; x |= (x + 0x80); return x; }
int foox(int x) { x &= 0xf; x |= (x ^ 0x80); return x; }

/* { dg-final { scan-assembler-not "add" } } */
/* { dg-final { scan-assembler-not "xor" } } */
/* { dg-final { scan-assembler-times "andi\t" 2 } } */
/* { dg-final { scan-assembler-times "ori\t" 2 } } */
