// go-system.h -- Go frontend inclusion of gcc header files   -*- C++ -*-
// Copyright (C) 2009, 2010 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#ifndef GO_SYSTEM_H
#define GO_SYSTEM_H

#include "config.h"

// These must be included before the #poison declarations in system.h.

#include <string>
#include <vector>
#include <tr1/unordered_map>

// We don't really need iostream, but some versions of gmp.h include
// it when compiled with C++, which means that we need to include it
// before the macro magic of safe-ctype.h, which is included by
// system.h.
#include <iostream>

// Some versions of gmp.h assume that #include <iostream> will define
// std::FILE.  This is not true with libstdc++ 4.3 and later.  This is
// fixed in GMP 4.3, but at this point we don't know which version of
// GMP is in use.  Since the top level configure script accepts GMP
// 4.2, at least for now we #include <cstdio> to ensure that GMP 4.2
// will work.  FIXME: This can be removed when we require GMP 4.3 or
// later.
#include <cstdio>

#ifndef ENABLE_BUILD_WITH_CXX
extern "C"
{
#endif

#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"

#include "diagnostic-core.h"	/* For error_at and friends.  */
#include "input.h"		/* For source_location.  */

#ifndef ENABLE_BUILD_WITH_CXX
} // End extern "C"
#endif

#endif // !defined(GO_SYSTEM_H)
