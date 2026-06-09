/* { dg-do run { target { lp64 } } } */
/* { dg-options "-mdejagnu-cpu=future -O2" } */
/* { dg-require-effective-target powerpc_future_hw } */

#include <amo.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* Test whether the compiler generates expected output for the Future ISA amo
   (atomic memory operations) functions  */

int
main (void)
{
  static uint32_t u32_test1_mem = 100;
  static uint32_t u32_test1_cond = 100;
  static uint32_t u32_test1_value = 250;
  static uint32_t u32_test1_expected_result = 100;
  static uint32_t u32_test1_actual_result;
  static uint32_t u32_test2_mem = 100;
  static uint32_t u32_test2_cond = 200;
  static uint32_t u32_test2_value = 250;
  static uint32_t u32_test2_expected_result = 100;
  static uint32_t u32_test2_actual_result;
  static uint64_t u64_mem = 300;
  static uint64_t u64_cond = 300;
  static uint64_t u64_value = 250;
  static uint64_t u64_expected_result = 300;
  static uint64_t u64_actual_result;

  u32_test1_actual_result = amo_lwat_cas_eq (&u32_test1_mem,
					     u32_test1_cond, u32_test1_value);
  u32_test2_actual_result = amo_lwat_cas_eq (&u32_test2_mem,
					     u32_test2_cond, u32_test2_value);
  u64_actual_result = amo_ldat_cas_eq (&u64_mem, u64_cond, u64_value);

  if (u32_test1_mem != u32_test1_value)
    abort ();

  if (u32_test1_actual_result != u32_test1_expected_result)
    abort ();

  if (u32_test2_mem != u32_test2_expected_result)
    abort ();

  if (u32_test2_actual_result != u32_test2_expected_result)
    abort ();

  if (u64_mem != u64_value)
    abort ();

  if (u64_actual_result != u64_expected_result)
    abort ();

  return 0;
}
