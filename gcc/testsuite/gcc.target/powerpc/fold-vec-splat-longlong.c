/* Verify that overloaded built-ins for vec_splat with long long
   inputs produce the right code.  */

/* { dg-do compile } */
/* { dg-options "-mvsx -O2" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

vector bool long long testb_00 (vector bool long long x) { return vec_splat (x, 0b00000); }
vector bool long long testb_01 (vector bool long long x) { return vec_splat (x, 0b00001); }

vector signed long long tests_00 (vector signed long long x) { return vec_splat (x, 0b00000); }
vector signed long long tests_01 (vector signed long long x) { return vec_splat (x, 0b00001); }

vector unsigned long long testu_00 (vector unsigned long long x) { return vec_splat (x, 0b00000); }
vector unsigned long long testu_01 (vector unsigned long long x) { return vec_splat (x, 0b00001); }

/* Similar test as above, but the source vector is a known constant. */
vector bool long long test_bll () { const vector bool long long y = {12, 23}; return vec_splat (y, 0b00001); }
vector signed long long test_sll () { const vector signed long long y = {34, 45}; return vec_splat (y, 0b00001); }

/* Assorted load instructions for the initialization with known constants. */
/* { dg-final { scan-assembler-times {\mlvx\M|\mlxvd2x\M|\mlxv\M|\mplxv\M|\mxxspltib\M} 2 } } */

/* xxpermdi for vec_splat of long long vectors.
 At the time of this writing, the number of xxpermdi instructions
 generated will vary depending on the target processor (p5/p6/p7/p8/...)
 and whether or not folding is enabled.
 So, ensure we have at least one hit.  */
/* { dg-final { scan-assembler "xxpermdi" } } */
