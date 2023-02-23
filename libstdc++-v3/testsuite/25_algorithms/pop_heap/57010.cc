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

// { dg-do run { target c++11 } }

#include <algorithm>
#include <functional>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>
#include <testsuite_rvalref.h>

using __gnu_test::test_container;
using __gnu_test::random_access_iterator_wrapper;
using __gnu_test::rvalstruct;

typedef test_container<rvalstruct, random_access_iterator_wrapper> container;

void test01()
{
  {
    rvalstruct makeheap[1];
    container makecon(makeheap, makeheap + 1);

    std::push_heap(makecon.begin(), makecon.end());
    std::pop_heap(makecon.begin(), makecon.end());
  }

  {
    rvalstruct makeheap[1];
    container makecon(makeheap, makeheap + 1);

    std::push_heap(makecon.begin(), makecon.end(), std::less<rvalstruct>());
    std::pop_heap(makecon.begin(), makecon.end(), std::less<rvalstruct>());
  }
}

int main()
{
  test01();
  return 0;
}
