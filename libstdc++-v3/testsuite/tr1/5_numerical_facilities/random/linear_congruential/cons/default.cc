// 2006-06-04  Stephen M. Webb <stephen.webb@bregmasoft.com>
//
// Copyright (C) 2006-2023 Free Software Foundation, Inc.
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

// 5.1.4.1 class template linear_congruential [tr.rand.eng.lcong]
// 5.1.4.1 [4]

#include <tr1/random>
#include <testsuite_hooks.h>

void
test01()
{
  using namespace std::tr1;

  linear_congruential<unsigned long, 48271, 0, 2147483647> x;
  VERIFY( x.min() == 1 );
  VERIFY( x.max() == 2147483647-1 );
  VERIFY( x() == 48271 );
}

int main()
{
  test01();
  return 0;
}
