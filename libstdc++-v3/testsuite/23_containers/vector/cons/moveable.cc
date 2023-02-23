// { dg-do run { target c++11 } }

// Copyright (C) 2005-2023 Free Software Foundation, Inc.
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


// NOTE: This makes use of the fact that we know how moveable
// is implemented on vector (via swap). If the implementation changed
// this test may begin to fail.

#include <vector>
#include <utility>
#include <testsuite_hooks.h>

void test01()
{
  std::vector<int> a,b;
  a.push_back(1);
  b = std::move(a);
  VERIFY( b.size() == 1 && b[0] == 1 && a.size() == 0 );

  std::vector<int> c(std::move(b));
  VERIFY( c.size() == 1 && c[0] == 1 );
  VERIFY( b.size() == 0 );
}

void test02()
{
  std::vector<bool> a,b;
  a.push_back(1);
  b = std::move(a);
  VERIFY( b.size() == 1 && b[0] == 1 && a.size() == 0 );

  std::vector<bool> c(std::move(b));
  VERIFY( c.size() == 1 && c[0] == 1 );
  VERIFY( b.size() == 0 );
}

int main(void)
{
  test01();
  test02();
  return 0;
}
