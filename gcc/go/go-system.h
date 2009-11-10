// go-system.h -- Go frontend inclusion of gcc header files   -*- C++ -*-

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef GO_SYSTEM_H
#define GO_SYSTEM_H

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

extern "C"
{

#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"
#include "tm.h"

#include "toplev.h"
#include "input.h"

} // End extern "C"

#endif // !defined(GO_SYSTEM_H)
