/* Copyright (C) 1997-2023 Free Software Foundation, Inc.

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
#include "tm.h"
#include "common/common-target.h"
#include "common/common-target-def.h"
#include "opts.h"
#include "flags.h"

/* Allow us to easily change the default for -malloc-cc.  */
#ifndef DEFAULT_NO_ALLOC_CC
#define MASK_DEFAULT_ALLOC_CC	MASK_ALLOC_CC
#else
#define MASK_DEFAULT_ALLOC_CC	0
#endif

#undef TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS		\
  (MASK_DEFAULT_ALLOC_CC			\
   | MASK_COND_MOVE				\
   | MASK_SCC					\
   | MASK_COND_EXEC				\
   | MASK_VLIW_BRANCH				\
   | MASK_MULTI_CE				\
   | MASK_NESTED_CE)

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;
