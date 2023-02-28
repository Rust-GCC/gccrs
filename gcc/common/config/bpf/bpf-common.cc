/* Common hooks for eBPF.
   Copyright (C) 2019-2023 Free Software Foundation, Inc.

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
#include "config/bpf/bpf-protos.h"

#undef TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS 0

/* Implement TARGET_OPTION_OPTIMIZATION_TABLE.  */
static const struct default_options bpf_option_optimization_table[] =
  {
    /* Enable -funroll-all-loops by default.  */
    { OPT_LEVELS_ALL, OPT_funroll_all_loops, NULL, 1 },
    { OPT_LEVELS_NONE, 0, NULL, 0 }
  };

#undef TARGET_OPTION_OPTIMIZATION_TABLE
#define TARGET_OPTION_OPTIMIZATION_TABLE bpf_option_optimization_table

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;
