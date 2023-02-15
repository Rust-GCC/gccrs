// { dg-do compile { target c++11 } }

// Copyright (C) 2012-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <cstdbool>

#ifndef __bool_true_false_are_defined
# error "The header <cstdbool> fails to define a macro named __bool_true_false_are_defined"
#endif

#ifdef bool
# error "The header <cstdbool> defines a macro named bool"
#endif

#ifdef true
# error "The header <cstdbool> defines a macro named true"
#endif

#ifdef false
# error "The header <cstdbool> defines a macro named false"
#endif
