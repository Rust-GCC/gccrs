// Copyright (C) 2020-2023 Free Software Foundation, Inc.
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

// { dg-do run { target { c++11 && { ! simulator } } } }

#include <vector>
#include <testsuite_hooks.h>

// Assume that 9MB is larger than the stack limit.
struct X { char data[9*1024*1024]; };

static_assert( std::is_trivial<X>::value, "" );

int main()
{
  std::vector<X> v(1);
  VERIFY( v.size() == 1 );
  v.clear();
  v.resize(2);
  VERIFY( v.size() == 2 );
}
