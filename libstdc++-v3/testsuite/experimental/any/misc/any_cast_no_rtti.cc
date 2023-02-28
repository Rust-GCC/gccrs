// { dg-options "-fno-rtti" }
// { dg-do run { target c++14 } }

// Copyright (C) 2014-2023 Free Software Foundation, Inc.
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

#include <experimental/any>
#include <string>
#include <cstring>
#include <testsuite_hooks.h>

using std::experimental::any;
using std::experimental::any_cast;

void test01()
{
  using std::experimental::bad_any_cast;
  any x(1);
  auto p = any_cast<double>(&x);
  VERIFY(p == nullptr);

  x = 1.0;
  p = any_cast<double>(&x);
  VERIFY(p != nullptr);

  x = any();
  p = any_cast<double>(&x);
  VERIFY(p == nullptr);

  try {
    any_cast<double>(x);
    VERIFY(false);
  } catch (const bad_any_cast&) {
  }
}

int main()
{
  test01();
}
