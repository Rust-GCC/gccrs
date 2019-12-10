/* { dg-do run } */
/* { dg-require-effective-target sse4 } */
/* { dg-options "-O2 -msse4.1" } */

#include "sse4_1-check.h"

#define VEC_T __m128
#define FP_T float

#define ROUND_INTRIN(x, mode) _mm_floor_ps(x)
#define ROUND_MODE _MM_FROUND_FLOOR
#define CHECK_ROUND_MODE 0x01

#define LOOP_INCREMENT 4
#define CHECK_LOOP_INCREMENT 1

#include "sse4_1-round.h"
