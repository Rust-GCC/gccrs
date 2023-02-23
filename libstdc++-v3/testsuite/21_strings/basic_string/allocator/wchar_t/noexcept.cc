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

// { dg-do compile { target c++11 } }
// COW strings don't support C++11 allocator propagation:
// { dg-require-effective-target cxx11_abi }

#include <string>
#include <testsuite_allocator.h>

using C = wchar_t;
const C c = L'a';
using traits = std::char_traits<C>;

using __gnu_test::propagating_allocator;

void test01()
{
  typedef std::allocator<C> alloc_type;
  typedef std::basic_string<C, traits, alloc_type> test_type;
  test_type v1;
  test_type v2;
  // this is a GNU extension for std::allocator
  static_assert( noexcept( v1 = std::move(v2) ), "Move assign cannot throw" );
  static_assert( noexcept( v1.swap(v2) ), "Swap cannot throw" );
}

void test02()
{
  typedef propagating_allocator<C, false> alloc_type;
  typedef std::basic_string<C, traits, alloc_type> test_type;
  test_type v1(alloc_type(1));
  test_type v2(alloc_type(2));
  static_assert( !noexcept( v1 = std::move(v2) ), "Move assign can throw" );
  static_assert( noexcept( v1.swap(v2) ), "Swap cannot throw" );
}

void test03()
{
  typedef propagating_allocator<C, true> alloc_type;
  typedef std::basic_string<C, traits, alloc_type> test_type;
  test_type v1(alloc_type(1));
  test_type v2(alloc_type(2));
  static_assert( noexcept( v1 = std::move(v2) ), "Move assign cannot throw" );
  static_assert( noexcept( v1.swap(v2) ), "Swap cannot throw" );
}
