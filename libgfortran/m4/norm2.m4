`/* Implementation of the NORM2 intrinsic
   Copyright (C) 2010-2023 Free Software Foundation, Inc.
   Contributed by Tobias Burnus  <burnus@net-b.de>

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

#include "libgfortran.h"'

include(iparm.m4)dnl
include(ifunction.m4)dnl
include(`mtype.m4')dnl

`#if defined (HAVE_'atype_name`) && defined (HAVE_'rtype_name`) && 'hasmathfunc(sqrt) && hasmathfunc(fabs)

mathfunc_macro

ARRAY_FUNCTION(0,
`	'rtype_name` scale;
	result = 0;
	scale = 1;',
`	  if (*src != 0)
	    {
	      'rtype_name` absX, val;
	      absX = MATHFUNC(fabs) (*src);
	      if (scale < absX)
		{
		  val = scale / absX;
		  result = 1 + result * val * val;
		  scale = absX;
		}
	      else
		{
		  val = absX / scale;
		  result += val * val;
		}
	    }',
`   result = scale * MATHFUNC(sqrt) (result);')

#endif
