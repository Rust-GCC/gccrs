// { dg-do run { target c++11 } }

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

// 25.3.9 [lib.alg.permutation.generators]

#include <algorithm>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>
#include <testsuite_rvalref.h>

using __gnu_test::test_container;
using __gnu_test::bidirectional_iterator_wrapper;
using __gnu_test::rvalstruct;
using std::next_permutation;

typedef test_container<rvalstruct, bidirectional_iterator_wrapper> Container;

void
test1()
{
  // Note: The standard is unclear on what should happen in this case.
  // This seems the only really sensible behaviour, and what is done.
  rvalstruct array[] = {0};
  Container con(array, array);
  VERIFY( !next_permutation(con.begin(), con.end()) );
}

void
test2()
{
  rvalstruct array[] = {0};
  Container con(array, array + 1);
  VERIFY( !next_permutation(con.begin(), con.end()) );
}

void
test3()
{
  rvalstruct array[] = {0, 3};
  Container con(array, array + 2);
  VERIFY( next_permutation(con.begin(), con.end()) );
  VERIFY( array[0] == 3 && array[1] == 0 );
  VERIFY( !next_permutation(con.begin(), con.end()) );
  VERIFY( array[0] == 0 && array[1] == 3 );
}

void
test4()
{
  int array[6] = {0, 1, 2, 3, 4, 5};
  for(int i = 0 ; i < 719; ++i)
    {
      rvalstruct temp_array[6];
      std::copy(array, array + 6, temp_array);
      Container con(temp_array, temp_array + 6);
      VERIFY( next_permutation(array, array + 6) );

// XXX FIXME:  parallel-mode should deal correctly with moveable-only types
// per C++0x, at minimum smoothly fall back to serial.
#ifndef _GLIBCXX_PARALLEL
      VERIFY( std::lexicographical_compare(temp_array, temp_array + 6, 
					   array, array + 6) );
#endif
    }
  VERIFY( !next_permutation(array,array + 6) );
  for(int i = 0; i < 6; ++i)
    VERIFY( array[i] == i );
}

bool
are_ordered(const rvalstruct& lhs, const rvalstruct& rhs)
{ return lhs < rhs; }

void
test5()
{
  int array[6] = {0, 1, 2, 3, 4, 5};
  for(int i = 0 ; i < 719; ++i)
    {
      rvalstruct temp_array[6];
      std::copy(array, array + 6, temp_array);
      Container con(temp_array, temp_array + 6);
      VERIFY( next_permutation(array, array + 6, are_ordered) );

// XXX FIXME:  parallel-mode should deal correctly with moveable-only types
// per C++0x, at minimum smoothly fall back to serial.
#ifndef _GLIBCXX_PARALLEL
      VERIFY( std::lexicographical_compare(temp_array, temp_array + 6,
					   array, array + 6, are_ordered) );
#endif
    }
  VERIFY( !next_permutation(array,array + 6, are_ordered) );
  for(int i = 0; i < 6; ++i)
    VERIFY( array[i] == i );
}

int main()
{
  test1();
  test2();
  test3();
  test4();
  test5();
  return 0;
}
