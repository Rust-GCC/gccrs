// { dg-do run { target c++11 } }
// { dg-require-cstdint "" }
//
// 2008-11-24  Edward M. Smith-Rowland <3dw4rd@verizon.net>
// 2012-09-04  Ulrich Drepper <drepper@gmail.com>
//
// Copyright (C) 2012-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <ext/random>
#include <testsuite_hooks.h>

void
test01()
{
  __gnu_cxx::beta_distribution<> u(1.5, 3.0);
  VERIFY( u.alpha() == 1.5 );
  VERIFY( u.beta() == 3.0 );
  VERIFY( u.min() == 0.0 );
  VERIFY( u.max() == 1.0 );
}

void
test02()
{
  using param_type = __gnu_cxx::beta_distribution<>::param_type;
  const param_type p(1.5, 3.0);
  __gnu_cxx::beta_distribution<> u(p);
  VERIFY( u.param() == p );
  VERIFY( u.param() != param_type{} );
  VERIFY( u.min() == 0.0 );
  VERIFY( u.max() == 1.0 );
}

int main()
{
  test01();
  test02();
}
