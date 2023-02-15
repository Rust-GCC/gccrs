// { dg-do run { target c++11 } }

// Copyright (C) 2009-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <type_traits>
#include <ext/throw_allocator.h>

void foo1()
{ 
  typedef __gnu_cxx::throw_value_limit value_type;
  value_type v1;
  value_type v2{v1};
  value_type v3{value_type()};
}

bool foo2()
{ 
  typedef __gnu_cxx::throw_value_limit value_type;
  bool b = std::is_convertible<value_type, value_type>::value;
  return b;
}

int main()
{
  foo1();
  foo2();
  return 0;
}
