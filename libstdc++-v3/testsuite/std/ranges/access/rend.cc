// Copyright (C) 2019 Free Software Foundation, Inc.
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

// { dg-options "-std=gnu++2a" }
// { dg-do run { target c++2a } }

#include <ranges>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>

struct R1
{
  int i = 0;
  int j = 0;

  constexpr const int* rbegin() const { return &i; }
  constexpr const int* rend() const { return &i + 1; }
  friend constexpr const int* rbegin(const R1&& r) { return &r.j; }
  friend constexpr const int* rend(const R1&& r) { return &r.j + 1; }
};

void
test01()
{
  constexpr R1 r;
  static_assert( std::ranges::rend(r) == &r.i + 1 );
  static_assert( std::ranges::rend(std::move(r)) == &r.j + 1 );
}

struct R2
{
  int a[2] = { };
  long l[2] = { };

  constexpr const int* begin() const { return a; }
  constexpr const int* end() const { return a + 2; }

  friend constexpr const long* begin(const R2&& r) { return r.l; }
  friend constexpr const long* end(const R2&& r) { return r.l + 2; }
};

void
test02()
{
  constexpr R2 r;
  static_assert( std::ranges::rend(r)
      == std::make_reverse_iterator(std::ranges::begin(r)) );
  static_assert( std::ranges::rend(std::move(r))
      == std::make_reverse_iterator(std::ranges::begin(std::move(r))) );
}

struct R3
{
  int i = 0;

  int* rbegin() noexcept { return &i + 1; }
  long* rend() noexcept { return nullptr; } // not a sentinel for rbegin()

  friend long* rbegin(R3&) noexcept { return nullptr; }
  friend int* rend(R3& r) { return &r.i; }
};

void
test03()
{
  R3 r;
  auto i1 = std::ranges::rbegin(r);
  auto i2 = rend(r);
  static_assert( std::sentinel_for<decltype(i2), decltype(i1)> );
  // VERIFY( std::ranges::rend(r) == r.i );
  // static_assert( !noexcept(std::ranges::rend(r)) );
}

void
test04()
{
  using __gnu_test::test_range;
  using __gnu_test::bidirectional_iterator_wrapper;

  int a[2] = { };
  test_range<int, bidirectional_iterator_wrapper> r(a);
  VERIFY( std::ranges::rend(r) == std::make_reverse_iterator(std::ranges::begin(r)) );
}

int
main()
{
  test01();
  test02();
  test03();
}
