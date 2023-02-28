// { dg-do compile { target c++11 } }

// Copyright (C) 2013-2023 Free Software Foundation, Inc.
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

#include <deque>

void test01()
{
  std::deque<int> d1, d2{5, 6};
  int n = 0;
  std::deque<int>::iterator it = d1.insert(d1.cbegin(), n);
  it = d1.insert(d1.cbegin(), 1);
  it = d1.insert(d1.cbegin(), {2, 3});
  it = d1.insert(d1.cbegin(), 1, 4);
  it = d1.insert(d1.cbegin(), d2.begin(), d2.end());
}
