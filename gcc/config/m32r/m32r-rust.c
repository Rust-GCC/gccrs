/* Subroutines for the Rust front end for the Renesas M32R architecture.
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

/* Implement TARGET_RUST_CPU_INFO for Renesas M32R targets.  */

void
m32r_rust_target_cpu_info (void)
{
  rust_add_target_info ("target_arch", "m32r");

  // llvm has no current or historical support for m32r, so I made up names
  if (TARGET_M32RX)
    rust_add_target_info ("target_feature", "m32rx");
  if (TARGET_M32R2)
    rust_add_target_info ("target_feature", "m32r2");
  if (!(TARGET_M32RX) && !(TARGET_M32R2))
    rust_add_target_info ("target_feature", "m32r");
  if (TARGET_ALIGN_LOOPS)
    rust_add_target_info ("target_feature", "align-loops");
  if (TARGET_BRANCH_COST)
    rust_add_target_info ("target_feature", "prefer-branch-over-cond");
  if (TARGET_DEBUG)
    rust_add_target_info ("target_feature", "debug");
  /* TODO: find way to have flush-func and flush-trap (they don't work well as
   * defines) */
  if (TARGET_LOW_ISSUE_RATE)
    rust_add_target_info ("target_feature", "issue-rate-1");
  else
    rust_add_target_info ("target_feature", "issue-rate-2");
  if (TARGET_MODEL_SMALL)
    rust_add_target_info ("target_feature", "model-small");
  else if (TARGET_MODEL_MEDIUM)
    rust_add_target_info ("target_feature", "model-medium");
  else if (TARGET_MODEL_LARGE)
    rust_add_target_info ("target_feature", "model-large");
  if (TARGET_SDATA_NONE)
    rust_add_target_info ("target_feature", "sdata-none");
  else if (TARGET_SDATA_SDATA)
    rust_add_target_info ("target_feature", "sdata-sdata");
  else if (TARGET_SDATA_USE)
    rust_add_target_info ("target_feature", "sdata-use");
  /* TODO: couldn't find any option on "-G" even though docs mention it - maybe
   * check that? */
}
