/* { dg-do compile { target rv64 } } */
/* { dg-options "-march=rv64gcb -mabi=lp64d" } */

unsigned long xor(unsigned long x) { return x ^ 0xfff0000000000fffULL; }
unsigned long ior(unsigned long x) { return x | 0xfff0000000000fffULL; }
unsigned long and(unsigned long x) { return x & 0xfff0000000000fffULL; }

/* { dg-final { scan-assembler-times "\\tli\\t" 3 } } */
/* { dg-final { scan-assembler-times "\\tsrli\t" 3 } } */

/* { dg-final { scan-assembler-times "\\txnor\t" 1 } } */
/* { dg-final { scan-assembler-times "\\torn\t" 1 } } */
/* { dg-final { scan-assembler-times "\\tandn\t" 1 } } */
