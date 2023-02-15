// { dg-do compile { target c++11 } }

// Copyright (C) 2008-2023 Free Software Foundation, Inc.
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

// Tuple

#include <tuple>

// libstdc++/35480
void test01()
{
  std::tuple<int> t1( 1 );
  std::tuple<int, int> t2( 1, 2 );
  if ( t1 < t2 ) {}   // { dg-error "here" }
  if ( t1 == t2 ) {}  // { dg-error "here" }
}
// { dg-prune-output "incomplete type" }
// { dg-excess-errors "body of constexpr function" }
