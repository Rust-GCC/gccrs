/* { dg-options "-march=la32v1.0 -mabi=ilp32s -fstack-protector-strong" } */
/* { dg-final { scan-assembler-not "ld\\\.d" } } */
/* { dg-final { scan-assembler-not "st\\\.d" } } */

void test()
{
	int x;
	asm("":"+m"(x));
}
