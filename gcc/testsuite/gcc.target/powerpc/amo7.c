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
static uint32_t u32_ld[2][2] = {
  { 10, 15 },			/* Decrement Bounded */
  { 10, 10 },			/* Decrement Bounded */
};

static uint32_t u32_actual_result[2];

static uint32_t u32_update[2] = {
  15 - 1,			/* Decrement Bounded */
  10,				/* Decrement Bounded */
};

static uint32_t u32_expected_result[2] = {
  15,				/* Decrement Bounded */
  INT_MIN,			/* Decrement Bounded */
};

/* 64-bit tests.  */
static uint64_t u64_ld[2][2] = {
  { 10, 15 },                   /* Decrement Bounded */
  { 10, 10 },                   /* Decrement Bounded */
};

static uint64_t u64_actual_result[2];

static uint64_t u64_update[2] = {
  15 - 1,                       /* Decrement Bounded */
  10,                           /* Decrement Bounded */
};

static uint64_t u64_expected_result[2] = {
  15,                           /* Decrement Bounded */
  INT64_MIN,                    /* Decrement Bounded */
};

int
main (void)
{
  size_t i;

  u32_actual_result[0] = amo_lwat_dec_bounded (&u32_ld[0][0]);
  u32_actual_result[1] = amo_lwat_dec_bounded (&u32_ld[1][0]);

  u64_actual_result[0] = amo_ldat_dec_bounded (&u64_ld[0][0]);
  u64_actual_result[1] = amo_ldat_dec_bounded (&u64_ld[1][0]);
 
  for (i = 0; i < 2; i++)
    {
      if (u32_actual_result[i] != u32_expected_result[i])
	abort ();

      if (u32_ld[i][1] != u32_update[i])
	abort ();

      if (u64_actual_result[i] != u64_expected_result[i])
	abort ();

      if (u64_ld[i][1] != u64_update[i])
	abort ();
    }

  return 0;
}
