/* { dg-do compile } */
/* { dg-options "-O2 -march=armv9-a+fprcvt" } */

#include <stdint.h>
#include <arm_neon.h>

#define GPF float32_t
#define SUFFIX(x) x##f
#define GPI int32_t

#include "fprcvt.x"

/* { dg-final { scan-assembler-times "fcvtzs\ts\[0-9\]+, *s\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "fcvtps\td\[0-9\]+, *s\[0-9\]" 1 } } */
/* { dg-final { scan-assembler-times "fcvtps\ts\[0-9\]+, *s\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "fcvtms\td\[0-9\]+, *s\[0-9\]" 1 } } */
/* { dg-final { scan-assembler-times "fcvtms\ts\[0-9\]+, *s\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "fcvtas\ts\[0-9\]+, *s\[0-9\]" 2 } } */
