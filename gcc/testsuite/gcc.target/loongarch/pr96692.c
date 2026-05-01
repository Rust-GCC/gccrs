/* { dg-do compile } */
/* { dg-options "-march=loongarch64 -O" { target lp64 } } */
/* { dg-options "-march=la32v1.0 -O" { target ilp32 } } */

int f(int a, int b, int c)
{
    return (a ^ b) ^ (a | c);
}

/* { dg-final { scan-assembler-times "xor\t" 1 } } */
/* { dg-final { scan-assembler-times "andn\t" 1 } } */
