/* { dg-do compile } */
/* { dg-options "-O3 -Wno-attributes -mfpmath=sse -mfma -mtune=generic -mno-fma4" } */
/* Disabling epilogues until we find a better way to deal with scans.  */
/* { dg-additional-options "--param vect-epilogues-nomask=0" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

typedef double adouble __attribute__((aligned(sizeof (double))));
#define TYPE adouble

#include "l_fma_2.h"

/* { dg-final { scan-assembler-times "vfmadd\[123\]+pd" 8 } } */
/* { dg-final { scan-assembler-times "vfmsub\[123\]+pd" 8 } } */
/* { dg-final { scan-assembler-times "vfnmadd\[123\]+pd" 8 } } */
/* { dg-final { scan-assembler-times "vfnmsub\[123\]+pd" 8 } } */
/* { dg-final { scan-assembler-times "vfmadd\[123\]+sd" 32 } } */
/* { dg-final { scan-assembler-times "vfmsub\[123\]+sd" 32 } } */
/* { dg-final { scan-assembler-times "vfnmadd\[123\]+sd" 32 } } */
/* { dg-final { scan-assembler-times "vfnmsub\[123\]+sd" 32 } } */
