/* { dg-do compile } */
/* { dg-additional-options "-march=armv8.2-a+sve" } */
#include <arm_sve.h>
void f(svfloat32x2_t *tp, const svfloat32x2_t *tuple, const svfloat32_t *vp) {
  *tp = svset2_f32(*tuple, 0, *vp);
}
