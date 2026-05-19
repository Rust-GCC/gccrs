/* { dg-do run { target { lp64 && p9vector_hw } } } */
/* { dg-options "-mdejagnu-cpu=power9 -O2" } */

/* This test uses uint64_t types. lp64 is added to avoid running it on 32-bit
    target. The test is skipped on 32-bit systems. */

#include <amo.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* Test whether the compiler generates expected code for the ISA 3.0 amo
   (atomic memory operations) functions  */

/* 32-bit tests.  */
static uint32_t u32_ld[4][2] = {
  { 10, 15 },			/* Increment Bounded */
  { 10, 10 },			/* Increment Bounded */
  { 10, 10 },			/* Increment Equal */
  { 10, 15 }			/* Increment Equal */
};

static uint32_t u32_actual_result[4];

static uint32_t u32_update[4] = {
  10 + 1,			/* Increment Bounded */
  10,				/* Increment Bounded */
  10 + 1,			/* Increment Equal */
  10				/* Increment Equal */
};

static uint32_t u32_expected_result[4] = {
  10,				/* Increment Bounded */
  INT_MIN,			/* Increment Bounded */
  10,				/* Increment Equal */
  INT_MIN			/* Increment Equal */
};

/* 64-bit tests.  */
static uint64_t u64_ld[4][2] = {
  { 10, 15 },			/* Increment Bounded */
  { 10, 10 },			/* Increment Bounded */
  { 10, 10 },			/* Increment Equal */
  { 10, 15 }			/* Increment Equal */
};

static uint64_t u64_actual_result[4];

static uint64_t u64_update[4] = {
  10 + 1,			/* Increment Bounded */
  10,				/* Increment Bounded */
  10 + 1,			/* Increment Equal */
  10				/* Increment Equal */
};

static uint64_t u64_expected_result[4] = {
  10,				/* Increment Bounded */
  INT64_MIN,			/* Increment Bounded */
  10,				/* Increment Equal */
  INT64_MIN			/* Increment Equal */
};

int
main (void)
{
  size_t i;

  u32_actual_result[0] = amo_lwat_inc_bounded (&u32_ld[0][0]);
  u32_actual_result[1] = amo_lwat_inc_bounded (&u32_ld[1][0]);
  u32_actual_result[2] = amo_lwat_inc_eq (&u32_ld[2][0]);
  u32_actual_result[3] = amo_lwat_inc_eq (&u32_ld[3][0]);

  u64_actual_result[0] = amo_ldat_inc_bounded (&u64_ld[0][0]);
  u64_actual_result[1] = amo_ldat_inc_bounded (&u64_ld[1][0]);
  u64_actual_result[2] = amo_ldat_inc_eq (&u64_ld[2][0]);
  u64_actual_result[3] = amo_ldat_inc_eq (&u64_ld[3][0]);

  for (i = 0; i < 4; i++)
    {
      if (u32_actual_result[i] != u32_expected_result[i])
	abort ();

      if (u32_ld[i][0] != u32_update[i])
	abort ();

      if (u64_actual_result[i] != u64_expected_result[i])
	abort ();

      if (u64_ld[i][0] != u64_update[i])
	abort ();
    }

  return 0;
}
