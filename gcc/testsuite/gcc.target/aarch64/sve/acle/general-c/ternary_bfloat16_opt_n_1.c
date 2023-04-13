/* { dg-do compile } */

#include <arm_sve.h>

#pragma GCC target ("arch=armv8.2-a+sve+bf16")

void
f1 (svbool_t pg, svuint8_t u8, svuint16_t u16, svint32_t s32,
    svbfloat16_t bf16, svfloat32_t f32, svfloat64_t f64, bfloat16_t bf)
{
  svbfdot (f32, bf16); /* { dg-error {too few arguments to function 'svbfdot'} } */
  svbfdot (f32, bf16, bf16, 0); /* { dg-error {too many arguments to function 'svbfdot'} } */
  svbfdot (0, bf16, bf16); /* { dg-error {passing 'int' to argument 1 of 'svbfdot', which expects an SVE vector type} } */
  svbfdot (pg, bf16, bf16); /* { dg-error {'svbfdot' has no form that takes 'svbool_t' arguments} } */
  svbfdot (u8, bf16, bf16); /* { dg-error {'svbfdot' has no form that takes 'svuint8_t' arguments} } */
  svbfdot (u16, bf16, bf16); /* { dg-error {'svbfdot' has no form that takes 'svuint16_t' arguments} } */
  svbfdot (f64, bf16, bf16); /* { dg-error {'svbfdot' has no form that takes 'svfloat64_t' arguments} } */
  svbfdot (f32, bf16, bf16);
  svbfdot (f32, 0, bf16); /* { dg-error {passing 'int' to argument 2 of 'svbfdot', which expects 'svbfloat16_t'} } */
  svbfdot (f32, f32, bf16); /* { dg-error {passing 'svfloat32_t' to argument 2 of 'svbfdot', which expects 'svbfloat16_t'} } */
  svbfdot (f32, bf16, 0);
  svbfdot (f32, bf16, f32); /* { dg-error {passing 'svfloat32_t' to argument 3 of 'svbfdot', which expects 'svbfloat16_t'} } */
  svbfdot (f32, bf16, bf);
}
