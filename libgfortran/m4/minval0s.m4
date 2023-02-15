`/* Implementation of the MAXLOC intrinsic
   Copyright (C) 2017-2023 Free Software Foundation, Inc.
   Contributed by Thomas Koenig

This file is part of the GNU Fortran runtime library (libgfortran).

Libgfortran is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

Libgfortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "libgfortran.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>'

include(iparm.m4)dnl
include(iforeach-s2.m4)dnl

`#if defined (HAVE_'atype_name`) && defined (HAVE_'rtype_name`)'

FOREACH_FUNCTION(
`  const atype_name *retval;
   retval = ret;'
,
`  if (compare_fcn (base, retval, len) < 0)
    {
      retval = base;
    }')

MASKED_FOREACH_FUNCTION(
`  const atype_name *retval;

  retval = ret;'
,
`  if (*mbase && compare_fcn (base, retval, len) < 0)
    {
      retval = base;
    }')

SCALAR_FOREACH_FUNCTION

#endif
