/* Subroutines for the Rust front end for the Xilinx MicroBlaze architecture.
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

/* Implement TARGET_RUST_CPU_INFO for Xilinx MicroBlaze targets.  */

void
microblaze_rust_target_cpu_info (void)
{
  rust_add_target_info ("target_arch", "microblaze");

  // llvm has no current or historical support for microblaze so I made up names
  // and features
  // TODO: maybe define subarches as features? probably needed, but not sure how
  // well it interacts
  if (TARGET_SOFT_FLOAT)
    rust_add_target_info ("target_feature", "soft-float");
  else
    rust_add_target_info ("target_feature", "hard-float");
  if (TARGET_SMALL_DIVIDES)
    rust_add_target_info ("target_feature", "small-divides");
  if (TARGET_MEMCPY)
    rust_add_target_info ("target_feature", "memcpy");
  if (TARGET_SOFT_MUL)
    rust_add_target_info ("target_feature", "xl-soft-mul");
  if (TARGET_REORDER)
    rust_add_target_info ("target_feature", "xl-reorder");
  if (TARGET_SOFT_DIV)
    rust_add_target_info ("target_feature", "xl-soft-div");
  if (TARGET_BARREL_SHIFT)
    rust_add_target_info ("target_feature", "xl-barrel-shift");
  if (TARGET_PATTERN_COMPARE)
    rust_add_target_info ("target_feature", "xl-pattern-compare");
  if (TARGET_STACK_CHECK)
    rust_add_target_info ("target_feature", "xl-stack-check");
  if (TARGET_XLGPOPT)
    rust_add_target_info ("target_feature", "xl-gp-opt");
  // TODO: ensure this below variable is accessible
  if (flag_zero_initialized_in_bss)
    rust_add_target_info ("target_feature", "no-clearbss");
  if (TARGET_MULTIPLY_HIGH)
    rust_add_target_info ("target_feature", "xl-multiply-high");
  if (TARGET_FLOAT_CONVERT)
    rust_add_target_info ("target_feature", "xl-float-convert");
  if (TARGET_FLOAT_SQRT)
    rust_add_target_info ("target_feature", "xl-float-sqrt");
  if (TARGET_XL_MODE_EXECUTABLE)
    rust_add_target_info ("target_feature", "xl-mode-executable");
  if (TARGET_XL_MODE_XMDSTUB)
    rust_add_target_info ("target_feature", "xl-mode-xmdstub");
  if (TARGET_XL_MODE_BOOTSTRAP)
    rust_add_target_info ("target_feature", "xl-mode-bootstrap");
  if (TARGET_XL_MODE_NOVECTORS)
    rust_add_target_info ("target_feature", "xl-mode-novectors");
  if (TARGET_PREFETCH)
    rust_add_target_info ("target_feature", "xl-prefetch");
  if (TARGET_PIC_DATA_TEXT_REL)
    rust_add_target_info ("target_feature", "pic-data-is-text-relative");
  // TODO find if value for xl-mode-xilkernel option is actually saved (it
  // doesn't appear to be)
}
