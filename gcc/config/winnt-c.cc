/* Default C-family target hooks initializer.
   Copyright (C) 2013-2023 Free Software Foundation, Inc.

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
#include "c-family/c-target.h"
#include "c-family/c-target-def.h"

static bool
winnt_implicit_extern_c (const char *ident)
{
  return !strcmp(ident, "wmain")
      || !strcmp(ident, "DllMain")
      || !strcmp(ident, "WinMain")
      || !strcmp(ident, "wWinMain");
}

#undef TARGET_CXX_IMPLICIT_EXTERN_C
#define TARGET_CXX_IMPLICIT_EXTERN_C winnt_implicit_extern_c

struct gcc_targetcm targetcm = TARGETCM_INITIALIZER;
