// Copyright (C) 2013-2023 Free Software Foundation, Inc.
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

// { dg-do run { target c++11 } }

#include <set>
#include <memory>
#include <testsuite_hooks.h>
#include <testsuite_allocator.h>

struct T { int i; };

bool operator<(T l, T r) { return l.i < r.i; }

using Cmp = std::less<T>;

using __gnu_test::SimpleAllocator;

template class std::multiset<T, Cmp, SimpleAllocator<T>>;

void test01()
{
  typedef SimpleAllocator<T> alloc_type;
  typedef std::allocator_traits<alloc_type> traits_type;
  typedef std::multiset<T, Cmp, alloc_type> test_type;
  test_type v(alloc_type{});
  v = { test_type::value_type{} };
  VERIFY( v.max_size() < traits_type::max_size(v.get_allocator()) );
}

int main()
{
  test01();
  return 0;
}
