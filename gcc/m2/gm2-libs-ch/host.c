/* host.c supply missing math routines.

Copyright (C) 2010-2023 Free Software Foundation, Inc.
Contributed by Gaius Mulley <gaius@glam.ac.uk>.

This file is part of GNU Modula-2.

GNU Modula-2 is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GNU Modula-2 is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "math.h"

#include "gm2-libs-host.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(HAVE_EXP10)
double
exp10 (double x)
{
  return exp (x * M_LN10);
}
#endif

#if !defined(HAVE_EXP10F)
float
exp10f (float x)
{
  return expf (x * M_LN10);
}
#endif

#if !defined(HAVE_EXP10L)
long double
exp10l (long double x)
{
  return expl (x * M_LN10);
}
#endif

#   ifdef __cplusplus
}
#   endif
