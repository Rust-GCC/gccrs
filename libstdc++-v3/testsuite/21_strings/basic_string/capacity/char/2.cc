// 1999-05-11 bkoz

// Copyright (C) 1999-2023 Free Software Foundation, Inc.
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

// 21.3.3 string capacity

#include <string>
#include <testsuite_hooks.h>

// libstdc++/4548
// http://gcc.gnu.org/ml/libstdc++/2001-11/msg00150.html
void test02()
{
  std::string str01 = "twelve chars";
  // str01 becomes shared
  std::string str02 = str01;
#if __cplusplus <= 201703L
  str01.reserve();
#else
  str01.shrink_to_fit(); // reserve is deprecated in C++20
#endif
  VERIFY( str01.capacity() >= 12 );
}

int main()
{
  test02();
  return 0;
}
