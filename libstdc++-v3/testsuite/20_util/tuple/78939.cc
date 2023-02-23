// Copyright (C) 2017-2023 Free Software Foundation, Inc.
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

// { dg-additional-options "-Wno-deprecated" { target c++2a } }
// { dg-do compile { target c++17 } }

// PR libstdc++/78939

#include <utility>

struct A { int i, j; };

int
test01()
{
  A a{};
  const auto [i, j] = a;
  return i + j;
}

int
test02()
{
  A a{};
  volatile auto [i, j] = a; // deprecated in C++20
  return i + j;
}

int
test03()
{
  A a{};
  const volatile auto [i, j] = a; // deprecated in C++20
  return i + j;
}
