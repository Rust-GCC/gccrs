/* Common hooks for TI PRU
   Copyright (C) 2014-2023 Free Software Foundation, Inc.

This file is part of GCC.

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
#include "diagnostic-core.h"
#include "tm.h"
#include "common/common-target.h"
#include "common/common-target-def.h"
#include "opts.h"
#include "flags.h"

#undef TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS		(MASK_OPT_LOOP)

#undef TARGET_EXCEPT_UNWIND_INFO
#define TARGET_EXCEPT_UNWIND_INFO sjlj_except_unwind_info

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;
