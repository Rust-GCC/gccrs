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

int
main (void)
{
  size_t i;
  static uint32_t u32_test1_expected_result[2] = { 5, 5 };
  static uint32_t u32_test1_mem[2] = { 3, 3 };
  static uint32_t u32_value_to_store = 5;

  static uint32_t u32_test2_mem[2] = { 3, 4 };
  static uint32_t u32_test2_expected_result[2] = { 3, 4 };

  static uint64_t u64_test_expected_result[2] = { 9, 9 };
  static uint64_t u64_test_mem[2]  = { 7, 7 };
  static uint64_t u64_value_to_store = 9;

  amo_stwat_twin (u32_test1_mem, u32_value_to_store);
  amo_stwat_twin (u32_test2_mem, u32_value_to_store);
  amo_stdat_twin (u64_test_mem, u64_value_to_store);

  for (i = 0; i < 2; i++)
    {
      if (u32_test1_mem[i] != u32_test1_expected_result[i])
	abort ();

      if (u32_test2_mem[i] != u32_test2_expected_result[i])
	abort ();

      if (u64_test_mem[i] != u64_test_expected_result[i])
	abort ();
    }

  return 0;
}
