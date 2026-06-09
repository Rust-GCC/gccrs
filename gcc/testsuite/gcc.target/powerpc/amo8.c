/* { dg-do compile { target { lp64 } } } */
/* { dg-options "-mdejagnu-cpu=future -O2" } */
/* { dg-require-effective-target powerpc_future_compile_ok } */

#include <amo.h>
#include <stdint.h>

/* Test whether the compiler generates expected code for the ISA <future> AMO
   (atomic memory operations) functions  */

uint32_t
do_lw_cs_eq (uint32_t *mem, uint32_t cond, uint32_t value)
{
  return amo_lwat_cas_eq (mem, cond, value);
}

int32_t
do_lw_scs_eq (int32_t *mem, int32_t cond, int32_t value)
{
  return amo_lwat_scas_eq (mem, cond, value);
}

uint64_t
do_ld_cs_eq (uint64_t *mem, uint64_t cond, uint64_t value)
{
  return amo_ldat_cas_eq (mem, cond, value);
}

int64_t
do_ld_scs_eq (int64_t *mem, int64_t cond, int64_t value)
{
  return amo_ldat_scas_eq (mem, cond, value);
}

/* { dg-final { scan-assembler-times {\mlwat\M}  2 } } */
/* { dg-final { scan-assembler-times {\mldat\M}  2 } } */
