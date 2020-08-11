/* Subroutines for the Rust front end on the CRIS architecture.
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

/* Implement TARGET_RUST_CPU_INFO for CRIS targets.  */

void
cris_rust_target_cpu_info (void)
{
  rust_add_target_info ("target_arch", "cris");

  // llvm seems to have no support for cris (nor historical support), so names
  // are made up by me
  // TODO maybe put in sub-arches as features? idk. might be useful in this case
  if (TARGET_MUL_BUG)
    rust_add_target_info ("target_feature", "mul-bug-workaround");
  if (TARGET_PDEBUG)
    rust_add_target_info ("target_feature", "pdebug");
  if (TARGET_CCINIT)
    rust_add_target_info ("target_feature", "cc-init");
  if (TARGET_SIDE_EFFECT_PREFIXES)
    rust_add_target_info ("target_feature", "side-effect-patterns");
  if (TARGET_STACK_ALIGN)
    rust_add_target_info ("target_feature", "stack-align");
  if (TARGET_DATA_ALIGN)
    rust_add_target_info ("target_feature", "data-align");
  if (TARGET_CONST_ALIGN)
    rust_add_target_info ("target_feature", "const-align");
  // TODO: figure out how gcc stores the 8-bit, 16-bit, and 32-bit options as
  // macros to add them
  if (TARGET_PROLOGUE_EPILOGUE)
    rust_add_target_info ("target_feature", "prologue-epilogue");
  // TODO: feature for max stack frame? might be too finicky
  if (TARGET_TRAP_USING_BREAK8)
    rust_add_target_info ("target_feature", "trap-using-break8");
  if (TARGET_TRAP_UNALIGNED_ATOMIC)
    rust_add_target_info ("target_feature", "trap-unaligned-atomic");
  if (TARGET_ATOMICS_MAY_CALL_LIBFUNCS)
    rust_add_target_info ("target_feature", "unaligned-atomic-may-use-library");
  /* TODO: maybe something about having individual instructions, e.g.
   * "TARGET_HAS_MUL_INSNS", "TARGET_HAS_LZ", other stuff in cris.h */
}
