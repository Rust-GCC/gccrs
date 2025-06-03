/* { dg-do compile } */
/* { dg-options "-O2 -mno-avx512f -mavx2 -mprefer-vector-width=128 -mtune=sandybridge" } */
/* Cope with --enable-frame-pointer, Solaris/x86 -mstackrealign default.  */
/* { dg-additional-options "-fomit-frame-pointer -mno-stackrealign" } */

extern char *dst;

void
foo (int x)
{
  __builtin_memset (dst, x, 66);
}

/* { dg-final { scan-assembler-times "vpbroadcastb\[ \\t\]+\[^\n\]*%xmm" 1 } } */
/* { dg-final { scan-assembler-times "vmovdqu\[ \\t\]+\[^\n\]*%xmm" 4 } } */
/* No need to dynamically realign the stack here.  */
/* { dg-final { scan-assembler-not "and\[^\n\r]*%\[re\]sp" } } */
/* Nor use a frame pointer.  */
/* { dg-final { scan-assembler-not "%\[re\]bp" } } */
