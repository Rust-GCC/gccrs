/* Generic hooks for the RTL middle-end.
   Copyright (C) 2004-2023 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "function.h"
#include "rtl.h"
#include "tree.h"
#include "insn-config.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "recog.h"
#include "rtlhooks-def.h"
#include "explow.h"
#include "target.h"


/* For speed, we will copy the RTX hooks struct member-by-member
   instead of doing indirect calls.  For these reason, we initialize
   *two* struct rtl_hooks globals: rtl_hooks is the one that is used
   to actually call the hooks, while general_rtl_hooks is used
   to restore the hooks by passes that modify them.  */

const struct rtl_hooks general_rtl_hooks = RTL_HOOKS_INITIALIZER;
struct rtl_hooks rtl_hooks = RTL_HOOKS_INITIALIZER;

rtx
gen_lowpart_general (machine_mode mode, rtx x)
{
  rtx result = gen_lowpart_common (mode, x);

  if (result)
    return result;
  /* Handle SUBREGs and hard REGs that were rejected by
     simplify_gen_subreg.  */
  else if (REG_P (x) || GET_CODE (x) == SUBREG)
    {
      result = gen_lowpart_common (mode, copy_to_reg (x));
      gcc_assert (result != 0);
      return result;
    }
  else
    {
      /* The only additional case we can do is MEM.  */
      gcc_assert (MEM_P (x));

      /* The following exposes the use of "x" to CSE.  */
      scalar_int_mode xmode;
      if (is_a <scalar_int_mode> (GET_MODE (x), &xmode)
	  && GET_MODE_SIZE (xmode) <= UNITS_PER_WORD
	  && TRULY_NOOP_TRUNCATION_MODES_P (mode, xmode)
	  && !reload_completed)
	return gen_lowpart_general (mode, force_reg (xmode, x));

      poly_int64 offset = byte_lowpart_offset (mode, GET_MODE (x));
      return adjust_address (x, mode, offset);
    }
}

rtx
reg_num_sign_bit_copies_general (const_rtx, scalar_int_mode, scalar_int_mode,
				 unsigned int *)
{
  return NULL;
}

rtx
reg_nonzero_bits_general (const_rtx, scalar_int_mode, scalar_int_mode,
			  unsigned HOST_WIDE_INT *)
{
  return NULL;
}

bool
reg_truncated_to_mode_general (machine_mode mode ATTRIBUTE_UNUSED,
			       const_rtx x ATTRIBUTE_UNUSED)
{
  return false;
}

/* Assuming that X is an rtx (e.g., MEM, REG or SUBREG) for a fixed-point
   number, return an rtx (MEM, SUBREG, or CONST_INT) that refers to the
   least-significant part of X.
   MODE specifies how big a part of X to return.

   If the requested operation cannot be done, 0 is returned.

   This is similar to gen_lowpart_general.  */

rtx
gen_lowpart_if_possible (machine_mode mode, rtx x)
{
  rtx result = gen_lowpart_common (mode, x);

  if (result)
    return result;
  else if (MEM_P (x))
    {
      /* This is the only other case we handle.  */
      poly_int64 offset = byte_lowpart_offset (mode, GET_MODE (x));
      rtx new_rtx = adjust_address_nv (x, mode, offset);
      if (! memory_address_addr_space_p (mode, XEXP (new_rtx, 0),
					 MEM_ADDR_SPACE (x)))
	return 0;

      return new_rtx;
    }
  else if (mode != GET_MODE (x) && GET_MODE (x) != VOIDmode && !SUBREG_P (x)
	   && validate_subreg (mode, GET_MODE (x), x,
			       subreg_lowpart_offset (mode, GET_MODE (x))))
    return gen_lowpart_SUBREG (mode, x);
  else
    return 0;
}

