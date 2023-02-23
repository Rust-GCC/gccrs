// { dg-do run { target c++11 } }
// { dg-require-cstdint "" }
// { dg-require-little-endian "" }
//
// 2010-03-16  Paolo Carlini  <paolo.carlini@oracle.com>
// 2012-08-28  Ulrich Drepper  <drepper@gmail.com>, adapted for SFMT
//
// Copyright (C) 2010-2023 Free Software Foundation, Inc.
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

// 26.5.3.2 Class template mersenne_twister_engine [rand.eng.mers]

#include <ext/random>
#include <testsuite_hooks.h>

void
test01()
{
  __gnu_cxx::sfmt19937 u, v;

  VERIFY( !(u != v) );

  u.discard(100);
  v.discard(100);

  VERIFY( !(u != v) );

  v.discard(1);

  VERIFY( u != v );
}

int main()
{
  test01();
  return 0;
}
