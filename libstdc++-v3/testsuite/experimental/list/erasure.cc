// { dg-do run { target c++14 } }

// Copyright (C) 2015-2023 Free Software Foundation, Inc.
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

#include <experimental/list>
#include <testsuite_hooks.h>

void
test01()
{
  auto is_odd = [](const int i) { return i % 2 != 0; };

  std::list<int> l{ 10, 11, 12, 14, 15, 17, 18, 19 };
  std::experimental::erase_if(l, is_odd);
  std::list<int> t{ 10, 12, 14, 18 };
  VERIFY( l == t );
}

void
test02()
{
  std::list<int> l{ 0, 11, 0, 0, 22, 33, 0, 0, 44, 0 };
  std::experimental::erase(l, 0);
  std::list<int> t{ 11, 22, 33, 44 };
  VERIFY( l == t );
  std::experimental::erase(l, 55);
  VERIFY( l == t );
}

int
main()
{
  test01();
  test02();

  return 0;
}
