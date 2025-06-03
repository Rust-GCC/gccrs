/* { dg-do compile }
/* { dg-require-effective-target p9vector_hw } */
/* { dg-options "-O2 -mdejagnu-cpu=power9" } */

#include <altivec.h>

vector unsigned int
rlmi_test_1 (vector unsigned int x, vector unsigned int y,
	     vector unsigned int z)
{
  return vec_rlmi (x, y, z);
}

vector unsigned long long
rlmi_test_2 (vector unsigned long long x, vector unsigned long long y,
	     vector unsigned long long z)
{
  return vec_rlmi (x, y, z);
}

vector unsigned int
vrlnm_test_1 (vector unsigned int x, vector unsigned int y)
{
  return vec_vrlnm (x, y);
}

vector unsigned long long
vrlnm_test_2 (vector unsigned long long x, vector unsigned long long y)
{
  return vec_vrlnm (x, y);
}

vector unsigned int
rlnm_test_1 (vector unsigned int x, vector unsigned int y,
	     vector unsigned int z)
{
  return vec_rlnm (x, y, z);
}

vector unsigned long long
rlnm_test_2 (vector unsigned long long x, vector unsigned long long y,
	     vector unsigned long long z)
{
  return vec_rlnm (x, y, z);
}

/* Expected code generation for rlmi_test_1 is vrlwmi.
   Expected code generation for rlmi_test_2 is vrldmi.
   Expected code generation for vrlnm_test_1 is vrlwnm.
   Expected code generation for vrlnm_test_2 is vrldnm.
   Expected code generation for the others is more complex, because
   the second and third arguments are combined by a shift and OR,
   and because there is no splat-immediate doubleword.
    - For rlnm_test_1: vspltisw, vslw, xxlor, vrlwnm.
    - For rlnm_test_2: xxspltib, vextsb2d, vsld, xxlor, vrldnm.
   There is a choice of splat instructions in both cases, so we
   just check for "splt".  In the past vextsb2d would be generated for
   rlnm_test_2, but the compiler no longer generates it.  */

/* { dg-final { scan-assembler-times "vrlwmi" 1 } } */
/* { dg-final { scan-assembler-times "vrldmi" 1 } } */
/* { dg-final { scan-assembler-times "splt" 2 } } */
/* { dg-final { scan-assembler-times "vslw" 1 } } */
/* { dg-final { scan-assembler-times "vsld" 1 } } */
/* { dg-final { scan-assembler-times "xxlor" 4 } } */
/* { dg-final { scan-assembler-times "vrlwnm" 2 } } */
/* { dg-final { scan-assembler-times "vrldnm" 2 } } */
