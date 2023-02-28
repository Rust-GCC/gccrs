/* Implementation of the IERRNO intrinsic.
   Copyright (C) 2005-2023 Free Software Foundation, Inc.
   Contributed by François-Xavier Coudert <coudert@clipper.ens.fr>

This file is part of the GNU Fortran 95 runtime library (libgfortran).

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

#include <errno.h>


/* INTEGER FUNCTION IERRNO()  */

extern GFC_INTEGER_4 ierrno_i4 (void);
export_proto(ierrno_i4);

GFC_INTEGER_4
ierrno_i4 (void)
{
  return (GFC_INTEGER_4) errno;
}

extern GFC_INTEGER_8 ierrno_i8 (void);
export_proto(ierrno_i8);

GFC_INTEGER_8
ierrno_i8 (void)
{
  return (GFC_INTEGER_8) errno;
}
