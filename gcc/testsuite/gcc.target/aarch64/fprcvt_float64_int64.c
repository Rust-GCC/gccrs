/* { dg-do compile } */
/* { dg-options "-O2 -march=armv9-a+fprcvt" } */

#include <stdint.h>
#include <arm_neon.h>

#define GPF float64_t
#define SUFFIX(x) x
#define GPI int64_t

#include "fprcvt.x"

/* { dg-final { scan-assembler-times "fcvtzs\td\[0-9\]+, *d\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "fcvtps\td\[0-9\]+, *d\[0-9\]" 3 } } */
/* { dg-final { scan-assembler-times "fcvtms\td\[0-9\]+, *d\[0-9\]" 3 } } */
/* { dg-final { scan-assembler-times "fcvtas\td\[0-9\]+, *d\[0-9\]" 2 } } */
