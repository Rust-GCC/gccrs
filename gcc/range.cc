/* Misc range functions.
   Copyright (C) 2017-2023 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldyh@redhat.com>.

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
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "gimple-pretty-print.h"
#include "fold-const.h"
#include "ssa.h"
#include "range.h"

value_range
range_zero (tree type)
{
  return value_range (build_zero_cst (type), build_zero_cst (type));
}

value_range
range_nonzero (tree type)
{
  return value_range (build_zero_cst (type), build_zero_cst (type),
		      VR_ANTI_RANGE);
}

value_range
range_positives (tree type)
{
  unsigned prec = TYPE_PRECISION (type);
  signop sign = TYPE_SIGN (type);
  return value_range (type, wi::zero (prec), wi::max_value (prec, sign));
}

value_range
range_negatives (tree type)
{
  unsigned prec = TYPE_PRECISION (type);
  signop sign = TYPE_SIGN (type);
  value_range r;
  if (sign == UNSIGNED)
    r.set_undefined ();
  else
    r = value_range (type, wi::min_value (prec, sign), wi::minus_one (prec));
  return r;
}
