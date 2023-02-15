// Copyright (C) 2009-2023 Free Software Foundation, Inc.
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

// { dg-do compile }

#include <valarray>

// PR libstdc++/40691
void test01()
{
  const std::valarray<int> vi(12);
  std::valarray<bool> vb1(12);
  std::valarray<bool> vb2(3);
  std::slice s(0,3,4);

  vb1 = !vi;
  vb2 = !(std::valarray<int>)vi[s];
  vb2 = !vi[s];
}

int main()
{
  test01();
  return 0;
}
