// { dg-do compile }

// Copyright (C) 2013-2025 Free Software Foundation, Inc.
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

#include <cmath>
using std::pow;
#include <tr1/cmath>
#include <testsuite_tr1.h>

void
test01()
{
  using namespace __gnu_test;

  float x = 2080703.375F;
#if __cplusplus < 201103L
  check_ret_type<float>(std::pow(x, 2));
#else
  // LWG 550 What should the return type of pow(float,int) be?
  check_ret_type<double>(std::pow(x, 2));
#endif
  check_ret_type<double>(std::tr1::pow(x, 2));
}
