/* { dg-do compile } */
/* { dg-options "-std=c99" } */

#include <arm_sve.h>

struct s { int i; };

void
f1 (svbool_t pg, signed char *s8_ptr, short *s16_ptr,
    svint8_t s8, svint16_t s16,
    svint32_t s32, svuint32_t u32, svfloat32_t f32,
    svint64_t s64, svuint64_t u64, svfloat64_t f64, struct s s)
{
  svld1sb_gather_offset (pg, s8_ptr, s32); /* { dg-warning {implicit declaration of function 'svld1sb_gather_offset'; did you mean 'svld1_gather_offset'} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr); /* { dg-error {too few arguments to function 'svld1sb_gather_offset_s32'} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr, s32, 0); /* { dg-error {too many arguments to function 'svld1sb_gather_offset_s32'} } */
  svld1sb_gather_offset_s32 (pg, s16_ptr, s32); /* { dg-warning {passing argument 2 of 'svld1sb_gather_s32offset_s32' from incompatible pointer type} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr, pg); /* { dg-error {passing 'svbool_t' to argument 3 of 'svld1sb_gather_offset_s32', which expects a vector of 32-bit integers} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr, s8); /* { dg-error {passing 'svint8_t' to argument 3 of 'svld1sb_gather_offset_s32', which expects a vector of 32-bit integers} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr, s16); /* { dg-error {passing 'svint16_t' to argument 3 of 'svld1sb_gather_offset_s32', which expects a vector of 32-bit integers} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr, s32);
  svld1sb_gather_offset_s32 (pg, s8_ptr, u32);
  svld1sb_gather_offset_s32 (pg, s8_ptr, f32); /* { dg-error {passing 'svfloat32_t' to argument 3 of 'svld1sb_gather_offset_s32', which expects a vector of 32-bit integers} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr, s64); /* { dg-error {passing 'svint64_t' to argument 3 of 'svld1sb_gather_offset_s32', which expects a vector of 32-bit integers} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr, u64); /* { dg-error {passing 'svuint64_t' to argument 3 of 'svld1sb_gather_offset_s32', which expects a vector of 32-bit integers} } */
  svld1sb_gather_offset_s32 (pg, s8_ptr, f64); /* { dg-error {passing 'svfloat64_t' to argument 3 of 'svld1sb_gather_offset_s32', which expects a vector of 32-bit integers} } */

  svld1sb_gather_offset_s32 (pg, 0, s32);
  svld1sb_gather_offset_s32 (pg, s, s32); /* { dg-error {'struct s' to argument 2 of 'svld1sb_gather_offset_s32', which expects a vector or pointer base address} } */

  svld1sb_gather_offset_s32 (pg, pg, 0); /* { dg-error {passing 'svbool_t' to argument 2 of 'svld1sb_gather_offset_s32', which expects 'svuint32_t'} } */
  svld1sb_gather_offset_s32 (pg, s32, 0); /* { dg-error {passing 'svint32_t' to argument 2 of 'svld1sb_gather_offset_s32', which expects 'svuint32_t'} } */
  svld1sb_gather_offset_s32 (pg, u32, 0);
  svld1sb_gather_offset_s32 (pg, u64, 0); /* { dg-error {passing 'svuint64_t' to argument 2 of 'svld1sb_gather_offset_s32', which expects 'svuint32_t'} } */
}
