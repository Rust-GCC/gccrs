// { dg-do run { target c++11 } }

// Copyright (C) 2005-2023 Free Software Foundation, Inc.
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

// 25.3.1 algorithms, sort()

#undef _GLIBCXX_CONCEPT_CHECKS

// XXX FIXME:  parallel-mode should deal correctly with moveable-only types
// per C++0x, at minimum smoothly fall back to serial.
#undef _GLIBCXX_PARALLEL

#include <algorithm>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>
#include <testsuite_rvalref.h>

using __gnu_test::test_container;
using __gnu_test::random_access_iterator_wrapper;
using __gnu_test::rvalstruct;
using std::partial_sort;

typedef test_container<rvalstruct, random_access_iterator_wrapper> Container;

const int A[] = {10, 20, 1, 11, 2, 12, 3, 13, 4, 14, 5, 15, 6, 16, 7, 
		 17, 8, 18, 9, 19};
const int N = sizeof(A) / sizeof(int);

// 25.3.1.1 sort()
void
test01()
{
  rvalstruct s1[N];
  std::copy(A, A + N, s1);
  Container con(s1, s1 + N);
  std::sort(con.begin(), con.end());
  VERIFY( s1[0].valid );
  for(int i = 1; i < N; ++i)
    VERIFY( s1[i].val>s1[i-1].val && s1[i].valid );
}

bool order(const rvalstruct& lhs, const rvalstruct& rhs)
{ return lhs < rhs; }

// 25.3.1.1 sort()
void
test02()
{
  rvalstruct s1[N];
  std::copy(A, A + N, s1);
  Container con(s1, s1 + N);
  std::sort(con.begin(), con.end(), order);
  VERIFY( s1[0].valid );
  for(int i = 1; i < N; ++i)
    VERIFY( s1[i].val>s1[i-1].val && s1[i].valid );
}

int
main()
{
  test01();
  test02();
  return 0;
}
