/* { dg-do compile { target { lp64 } } } */
/* { dg-options "-mdejagnu-cpu=power9 -O2" } */

/* Verify whether the compiler generates expected code for the ISA 3.0 amo
   (atomic memory operations) functions  */

#include <amo.h>
#include <stdint.h>

uint32_t
do_lw_cs_ne (uint32_t *mem, uint32_t cond, uint32_t value)
{
  return amo_lwat_cas_neq (mem, cond, value);
}

int32_t
do_lw_scs_ne (int32_t *mem, int32_t cond, int32_t value)
{
  return amo_lwat_scas_neq (mem, cond, value);
}

uint32_t
do_lw_inc_equal (uint32_t *mem)
{
  return amo_lwat_inc_eq (mem);
}

int32_t
do_lw_sinc_equal (int32_t *mem)
{
  return amo_lwat_sinc_eq (mem);
}

uint32_t
do_lw_inc_bounded (uint32_t *mem)
{
  return amo_lwat_inc_bounded (mem);
}

int32_t
do_lw_sinc_bounded (int32_t *mem)
{
  return amo_lwat_sinc_bounded (mem);
}

uint32_t
do_lw_dec_bounded (uint32_t *mem)
{
  return amo_lwat_dec_bounded (mem);
}

int32_t
do_lw_sdec_bounded (int32_t *mem)
{
  return amo_lwat_sdec_bounded (mem);
}

uint64_t
do_ld_cs_ne (uint64_t *mem, uint64_t cond, uint64_t value)
{
  return amo_ldat_cas_neq (mem, cond, value);
}

int64_t
do_ld_scs_ne (int64_t *mem, int64_t cond, int64_t value)
{
  return amo_ldat_scas_neq (mem, cond, value);
}

uint64_t
do_ld_inc_equal (uint64_t *mem)
{
  return amo_ldat_inc_eq (mem);
}

int64_t
do_ld_sinc_equal (int64_t *mem)
{
  return amo_ldat_sinc_eq (mem);
}

uint64_t
do_ld_inc_bounded (uint64_t *mem)
{
  return amo_ldat_inc_bounded (mem);
}

int64_t
do_ld_sinc_bounded (int64_t *mem)
{
  return amo_ldat_sinc_bounded (mem);
}

uint64_t
do_ld_dec_bounded (uint64_t *mem)
{
  return amo_ldat_dec_bounded (mem);
}

int64_t
do_ld_sdec_bounded (int64_t *mem)
{
  return amo_ldat_sdec_bounded (mem);
}

void
do_sw_twin (uint32_t *mem, uint32_t value)
{
  amo_stwat_twin (mem, value);
}

void
do_sw_stwin (int32_t *mem, int32_t value)
{
  amo_stwat_stwin (mem, value);
}

void
do_sd_twin (uint64_t *mem, uint64_t value)
{
  amo_stdat_twin (mem, value);
}

void
do_sd_stwin (int64_t *mem, int64_t value)
{
  amo_stdat_stwin (mem, value);
}

/* { dg-final { scan-assembler-times {\mldat\M}  8 } } */
/* { dg-final { scan-assembler-times {\mlwat\M}  8 } } */
/* { dg-final { scan-assembler-times {\mstdat\M}  2 } } */
/* { dg-final { scan-assembler-times {\mstwat\M}  2 } } */
