/* Subroutines for the Rust front end on the PowerPC architecture.
   Copyright (C) 2020 Free Software Foundation, Inc.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tm_p.h"
#include "rust/rust-target.h"
#include "rust/rust-target-def.h"

/* Implement TARGET_RUST_CPU_INFO for PowerPC targets.  */

void
rs6000_rust_target_cpu_info (void)
{
  /* note that rustc makes no arch distinction between powerpc64 and powerpc64
   * little endian */
  if (TARGET_64BIT)
    rust_add_target_info ("target_arch", "powerpc64");
  else
    rust_add_target_info ("target_arch", "powerpc");

  // features from rustc - feature complete (although with caveats)
  if (TARGET_ALTIVEC)
    rust_add_target_info ("target_feature", "altivec");
  /* I can't find any separate gcc equivalent to "power8-altivec" in llvm, but
   * power8-vector has it as a prerequisite, so just implicitly enable it when
   * enabling the vector. TODO search for it. */
  if (TARGET_P8_VECTOR)
    {
      rust_add_target_info ("target_feature", "power8-vector");
      rust_add_target_info ("target_feature", "power8-altivec");
    }
  // Same implicit enabling of power9-altivec happens with power9-vector.
  if (TARGET_P9_VECTOR)
    {
      rust_add_target_info ("target_feature", "power9-vector");
      rust_add_target_info ("target_feature", "power9-altivec");
    }
  if (TARGET_VSX)
    rust_add_target_info ("target_feature", "vsx");

  // options from llvm
  if (TARGET_64BIT)
    rust_add_target_info ("target_feature", "64bit");
  if (!TARGET_SOFT_FLOAT)
    rust_add_target_info ("target_feature", "hard-float");
  if (TARGET_FRE)
    rust_add_target_info ("target_feature", "fre");
  if (TARGET_FRES)
    rust_add_target_info ("target_feature", "fres");
  if (TARGET_FRSQRTE)
    rust_add_target_info ("target_feature", "frsqrte");
  if (TARGET_FRSQRTES)
    rust_add_target_info ("target_feature", "frsqrtes");
  if (TARGET_RECIP_PRECISION)
    rust_add_target_info ("target_feature", "recipprec");
  if (TARGET_STFIWX)
    rust_add_target_info ("target_feature", "stfiwx");
  if (TARGET_LFIWAX)
    rust_add_target_info ("target_feature", "lfiwax");
  if (TARGET_FPRND)
    rust_add_target_info ("target_feature", "fprnd");
  if (TARGET_ISEL)
    rust_add_target_info ("target_feature", "isel");
  if (TARGET_LDBRX)
    rust_add_target_info ("target_feature", "ldbrx");
  if (TARGET_CMPB)
    rust_add_target_info ("target_feature", "cmpb");
  if (TARGET_SECURE_PLT)
    rust_add_target_info ("target_feature", "secure-plt");
  if (TARGET_CRYPTO)
    rust_add_target_info ("target_feature", "crypto");
  if (TARGET_DIRECT_MOVE)
    rust_add_target_info ("target_feature", "direct-move");
  if (rs6000_default_long_calls)
    rust_add_target_info ("target_feature", "longcall");
  if (TARGET_HTM)
    rust_add_target_info ("target_feature", "htm");
  /* TODO: "fusion" is currently only added implicitly, as can't find gcc option
   * for only fusion */
  if (TARGET_P8_FUSION)
    {
      rust_add_target_info ("target_feature", "fuse-addi-load");
      rust_add_target_info ("target_feature", "fusion");
    }
  if (TARGET_P8_FUSION_SIGN)
    {
      rust_add_target_info ("target_feature", "fuse-addis-load");
      rust_add_target_info ("target_feature", "fusion");
    }
  if (TARGET_FLOAT128_KEYWORD)
    rust_add_target_info ("target_feature", "float128");
  // TODO: make some processors have slow-popcntd instead of this
  if (TARGET_POPCNTD)
    rust_add_target_info ("target_feature", "popcntd");
  if (TARGET_PREFIXED_ADDR)
    rust_add_target_info ("target_feature", "prefix-instrs");
  if (TARGET_PCREL)
    rust_add_target_info ("target_feature", "pcrelative-memops");

  /* TODO: investigate whether 64bitregs is supported by gcc (64-bit registers
   * in ppc32), same with crbits (use condition register bits individually), fpu
   * (classic fp instructions - fp is supported, but can it be turned off as
   * option? could "hard float" replace this concept?), spe, mfocrf, fsqrt,
   * fcpsgn, fpcvt (related to TARGET_FLOAT128_CVT? TARGET_LFIWZX?), bpermd,
   * extdiv, icbt, booke, msync, e500, ppc4xx, ppc6xx, two-const-nr,
   * partword-atomics, invariant-function-descriptors,
   * allow-unaligned-fp-access, ppc-prera-sched, ppc-postra-sched,
   * isa-v30-instructions, isa-v31-instructions, power10-vector,
   * vectors-use-two-units, paired-vector-memops, predictable-select-expensive
   */
}
