// { dg-do run { target c++11 } }

// 2007-10-12  Paolo Carlini  <pcarlini@suse.de>
//
// Copyright (C) 2007-2023 Free Software Foundation, Inc.
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

// 25.3.6 Heap operations [lib.alg.heap.operations]

#include <algorithm>
#include <functional>
#include <testsuite_hooks.h>

int A[] = {9, 8, 6, 7, 7, 5, 5, 3, 6, 4, 1, 2, 3, 4};
int B[] = {1, 3, 2, 4, 4, 6, 3, 5, 5, 7, 7, 6, 8, 9};
const int N = sizeof(A) / sizeof(int);

void
test01()
{
  for (int i = 0; i <= N; ++i)
    {
      VERIFY( std::is_heap(A, A + i) );
      VERIFY( std::is_heap(A, A + i, std::less<int>()) );
      VERIFY( std::is_heap(B, B + i, std::greater<int>()) );
      VERIFY( (i < 2) || !std::is_heap(B, B + i) );
    }
}

int
main()
{
  test01();
  return 0;
}
