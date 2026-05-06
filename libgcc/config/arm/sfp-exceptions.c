/*
 * Copyright (C) 2026 Free Software Foundation, Inc.
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3, or (at your option) any
 * later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Under Section 7 of GPL version 3, you are granted additional
 * permissions described in the GCC Runtime Library Exception, version
 * 3.1, as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License and
 * a copy of the GCC Runtime Library Exception along with this program;
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#if __ARM_FP
#include "sfp-machine.h"

/* _BitInt support for the arm port will use software emulation for conversions
   to and from any floating point type to _BitInt.  These operations can throw
   exceptions and to make sure the exception state is set correctly when
   floating point hardware is available we execute floating point instructions
   that we know will set the exception state according to the exception bits
   set in _FEX.  */

void
__sfp_handle_exceptions (int _fex)
{
  volatile float fp_max = __FLT_MAX__;
  volatile float fp_min = __FLT_MIN__;
  volatile float fp_1e32 = 1.0e32f;
  volatile float fp_zero = 0.0;
  volatile float fp_one = 1.0;

  if ((_fex & FP_EX_INVALID) || (_fex & FP_EX_DIVZERO))
    fp_zero = fp_zero / fp_zero;
  if (_fex & FP_EX_OVERFLOW)
    fp_1e32 += fp_max;
  if (_fex & FP_EX_UNDERFLOW)
    fp_min *= fp_min;
  if (_fex & FP_EX_INEXACT)
    fp_max -= fp_one;
}
#endif
