/* { dg-do compile } */
/* { dg-options "-O2 -march=armv9-a+fprcvt" } */

#include <stdint.h>
#include <arm_neon.h>

#define GPF float32_t
#define SUFFIX(x) x##f
#define GPI int64_t

#include "fprcvt.x"

/* { dg-final { scan-assembler-times "fcvtzs\td\[0-9\]+, *s\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "fcvtps\td\[0-9\]+, *s\[0-9\]" 3 } } */
/* { dg-final { scan-assembler-times "fcvtms\td\[0-9\]+, *s\[0-9\]" 3 } } */
/* { dg-final { scan-assembler-times "fcvtas\td\[0-9\]+, *s\[0-9\]" 2 } } */
