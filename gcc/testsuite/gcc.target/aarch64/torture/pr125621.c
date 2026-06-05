/* { dg-do compile } */
/* { dg-options "-march=armv8.2-a+sve -fsanitize=undefined" } */
#include <arm_sve.h>

long foo(long x) {
  return (long)svcntd() * x;
}
