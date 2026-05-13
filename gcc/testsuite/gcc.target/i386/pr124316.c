/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-mptwrite" } */
#include <x86gprintrin.h>

unsigned long long ull;
unsigned u;
void foo()
{
    _ptwrite64(ull);
    _ptwrite32(u);
}

/* { dg-final { scan-assembler-times "ptwritel" 1 } } */
/* { dg-final { scan-assembler-times "ptwriteq" 1 } } */
