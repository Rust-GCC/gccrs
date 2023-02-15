// Copyright (C) 2019-2023 Free Software Foundation, Inc.
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
// { dg-do compile { target c++2a } }

#include <functional>

struct add
{
  constexpr int operator()(int i, int j) const { return i + j; }
};

constexpr int
test01(int i, int j)
{
  add a;
  return std::bind(a, i, std::placeholders::_1)(j);
}

static_assert( test01(1, 2) == 3 );

constexpr int
test02(int i, int j)
{
  return std::bind<int>(add{}, i, std::placeholders::_1)(j);
}

static_assert( test02(4, 5) == 9 );
