// Copyright (C) 2006-2023 Free Software Foundation, Inc.
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

// 6.3.4.5  Class template unordered_multiset

#include <tr1/unordered_set>
#include <testsuite_hooks.h>
#include <testsuite_allocator.h>

// libstdc++/29134
void test01()
{
  std::tr1::unordered_multiset<int> ums;

  std::allocator<std::tr1::__detail::_Hash_node<int, false> > a;
  VERIFY( ums.max_size() == __gnu_test::max_size(a) );
}

int main()
{
  test01();
  return 0;
}
