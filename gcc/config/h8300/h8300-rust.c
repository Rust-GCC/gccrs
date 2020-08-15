/* Subroutines for the Rust front end for the Renesas H8/300 architecture.
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

/* Implement TARGET_RUST_CPU_INFO for Renesas H8/300 targets.  */

void
h8300_rust_target_cpu_info (void)
{
  rust_add_target_info ("target_arch", "h8300");

  /* note that llvm has no support, current or historical, so names made up by
   * me (via option names) */
  /* TODO: maybe rename some features as kind of bad names - e.g. "s" is
   * horrible */
  if (TARGET_H8300S_1)
    rust_add_target_info ("target_feature", "s");
  if (TARGET_H8300SX)
    rust_add_target_info ("target_feature", "sx");
  if (TARGET_MAC)
    rust_add_target_info ("target_feature", "s2600");
  if (TARGET_INT32)
    rust_add_target_info ("target_feature", "int32");
  if (TARGET_ADDRESSES)
    rust_add_target_info ("target_feature", "addresses");
  if (TARGET_QUICKCALL)
    rust_add_target_info ("target_feature", "quickcall");
  if (TARGET_SLOWBYTE)
    rust_add_target_info ("target_feature", "slowbyte");
  if (TARGET_RELAX)
    rust_add_target_info ("target_feature", "relax");
  if (TARGET_H8300H)
    rust_add_target_info ("target_feature", "h");
  if (TARGET_NORMAL_MODE)
    rust_add_target_info ("target_feature", "n");
  if (TARGET_ALIGN_300)
    rust_add_target_info ("target_feature", "align-300");
  if (TARGET_EXR)
    rust_add_target_info ("target_feature", "exr");
  if (TARGET_NEXR)
    rust_add_target_info ("target_feature", "nexr");
}
