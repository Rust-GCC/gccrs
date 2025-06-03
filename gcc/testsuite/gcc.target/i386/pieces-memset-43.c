/* { dg-do compile } */
/* { dg-options "-O2 -mno-avx2 -mavx -mprefer-vector-width=128 -mtune=sandybridge" } */

extern char *dst;

void
foo (void)
{
  __builtin_memset (dst, -1, 33);
}

/* { dg-final { scan-assembler-times "vpcmpeqd\[ \\t\]+\[^\n\]*%xmm" 1 } } */
/* { dg-final { scan-assembler-times "vmovdqu\[ \\t\]+\[^\n\]*%xmm" 2 } } */
/* No need to dynamically realign the stack here.  */
/* { dg-final { scan-assembler-not "and\[^\n\r]*%\[re\]sp" } } */
/* Nor use a frame pointer.  */
/* { dg-final { scan-assembler-not "%\[re\]bp" } } */
