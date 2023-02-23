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

// { dg-options "-std=gnu++2a" }
// { dg-do run { target c++2a } }

#include <system_error>
#include <testsuite_error.h>

void
test01()
{
  __gnu_test::test_category c1;
  VERIFY( std::is_eq(c1 <=> c1) );
  __gnu_test::test_derived_category c2;
  VERIFY( std::is_neq(c1 <=> c2) );
}

void
test02()
{
  __gnu_test::test_category c1;
  VERIFY( c1 <= c1 );
  VERIFY( c1 >= c1 );
  __gnu_test::test_derived_category c2;
  VERIFY( (c1 < c2) || (c2 < c1) );
  VERIFY( (c1 > c2) || (c2 > c1) );
}

int main()
{
  test01();
  test02();
}
