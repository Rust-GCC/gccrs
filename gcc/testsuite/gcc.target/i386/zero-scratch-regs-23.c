/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fzero-call-used-regs=all -march=corei7 -mavx512f" } */

void
foo (void)
{
}

/* { dg-final { scan-assembler "vzeroall" } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm16, %zmm16, %zmm16" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm17, %zmm17, %zmm17" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm18, %zmm18, %zmm18" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm19, %zmm19, %zmm19" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm20, %zmm20, %zmm20" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm21, %zmm21, %zmm21" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm22, %zmm22, %zmm22" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm23, %zmm23, %zmm23" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm24, %zmm24, %zmm24" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm25, %zmm25, %zmm25" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm26, %zmm26, %zmm26" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm27, %zmm27, %zmm27" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm28, %zmm28, %zmm28" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm29, %zmm29, %zmm29" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm30, %zmm30, %zmm30" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "vpxord\[ \t\]+%zmm31, %zmm31, %zmm31" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "fldz" 8 } } */
/* { dg-final { scan-assembler-times "fstp\[ \t\]+%st\\(0\\)" 8 } } */
/* { dg-final { scan-assembler-not "%xmm" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%eax, %eax" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%edx, %edx" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%ecx, %ecx" } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%esi, %esi" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%edi, %edi" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r8d, %r8d" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r9d, %r9d" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r10d, %r10d" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "xorl\[ \t\]+%r11d, %r11d" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k0, %k0, %k0" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k1, %k1, %k1" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k2, %k2, %k2" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k3, %k3, %k3" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k4, %k4, %k4" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k5, %k5, %k5" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k6, %k6, %k6" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "kxorw\[ \t\]+%k7, %k7, %k7" { target { ! ia32 } } } } */
