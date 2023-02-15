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

#include <map>
#include <testsuite_hooks.h>
#include <testsuite_allocator.h>

struct T { int i; };

bool operator<(T l, T r) { return l.i < r.i; }

using Cmp = std::less<T>;

struct U { };

using __gnu_test::propagating_allocator;

// It is undefined behaviour to swap() containers wth unequal allocators
// if the allocator doesn't propagate, so ensure the allocators compare
// equal, while still being able to test propagation via get_personality().
bool
operator==(const propagating_allocator<std::pair<const T, U>, false>&,
           const propagating_allocator<std::pair<const T, U>, false>&)
{
  return true;
}

bool
operator!=(const propagating_allocator<std::pair<const T, U>, false>&,
           const propagating_allocator<std::pair<const T, U>, false>&)
{
  return false;
}

void test01()
{
  typedef propagating_allocator<std::pair<const T, U>, false> alloc_type;
  typedef std::multimap<T, U, Cmp, alloc_type> test_type;
  test_type v1(alloc_type(1));
  v1 = { test_type::value_type{} };
  test_type v2(alloc_type(2));
  v2 = { test_type::value_type{} };
  std::swap(v1, v2);
  VERIFY(1 == v1.get_allocator().get_personality());
  VERIFY(2 == v2.get_allocator().get_personality());
  // swap back so assertions in uneq_allocator::deallocate don't fail
  std::swap(v1, v2);
}

void test02()
{
  typedef propagating_allocator<std::pair<const T, U>, true> alloc_type;
  typedef std::multimap<T, U, Cmp, alloc_type> test_type;
  test_type v1(alloc_type(1));
  v1 = { test_type::value_type{} };
  test_type v2(alloc_type(2));
  v2 = { test_type::value_type{} };
  std::swap(v1, v2);
  VERIFY(2 == v1.get_allocator().get_personality());
  VERIFY(1 == v2.get_allocator().get_personality());
}

int main()
{
  test01();
  test02();
  return 0;
}
