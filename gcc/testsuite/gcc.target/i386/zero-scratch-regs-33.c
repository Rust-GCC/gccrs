/* { dg-do compile { target { *-*-linux* && { ! ia32 } } } } */
/* { dg-options "-O2 -fzero-call-used-regs=all -march=corei7 -mavx512f -mapxf" } */

void
foo (void)
{
}

/* { dg-final { scan-assembler "vzeroall" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm16, %zmm16, %zmm16" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm17, %zmm17, %zmm17" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm18, %zmm18, %zmm18" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm19, %zmm19, %zmm19" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm20, %zmm20, %zmm20" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm21, %zmm21, %zmm21" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm22, %zmm22, %zmm22" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm23, %zmm23, %zmm23" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm24, %zmm24, %zmm24" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm25, %zmm25, %zmm25" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm26, %zmm26, %zmm26" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm27, %zmm27, %zmm27" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm28, %zmm28, %zmm28" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm29, %zmm29, %zmm29" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm30, %zmm30, %zmm30" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm31, %zmm31, %zmm31" } } */
/* { dg-final { scan-assembler-times "fldz" 8 } } */
/* { dg-final { scan-assembler-times "fstp\[ \t\]+%st\\(0\\)" 8 } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%eax, %eax" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%edx, %edx" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%ecx, %ecx" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%esi, %esi" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%edi, %edi" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r8d, %r8d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r9d, %r9d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r10d, %r10d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r11d, %r11d" } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k0, %k0, %k0" } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k1, %k1, %k1" } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k2, %k2, %k2" } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k3, %k3, %k3" } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k4, %k4, %k4" } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k5, %k5, %k5" } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k6, %k6, %k6" } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k7, %k7, %k7" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r16d, %r16d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r17d, %r17d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r18d, %r18d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r19d, %r19d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r20d, %r20d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r21d, %r21d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r22d, %r22d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r23d, %r23d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r24d, %r24d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r25d, %r25d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r26d, %r26d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r27d, %r27d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r28d, %r28d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r29d, %r29d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r30d, %r30d" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r31d, %r31d" } } */
