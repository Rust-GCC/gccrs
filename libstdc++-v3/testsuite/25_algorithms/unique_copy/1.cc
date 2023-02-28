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

// 25.2.8 [lib.alg.unique]

#include <algorithm>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>

using __gnu_test::test_container;
using __gnu_test::input_iterator_wrapper;
using __gnu_test::forward_iterator_wrapper;
using __gnu_test::output_iterator_wrapper;
using std::unique_copy;

typedef test_container<int, input_iterator_wrapper> Icontainer;
typedef test_container<int, forward_iterator_wrapper> Fcontainer;
typedef test_container<int, output_iterator_wrapper> Ocontainer;

int array1[] = {0, 0, 0, 1, 1, 1};
int array2[2];

void 
test1()
{
  Icontainer con1(array1, array1);
  Ocontainer con2(array2, array2);
  VERIFY( unique_copy(con1.begin(), con1.end(), con2.begin()).ptr == array2 );
}

void
test2()
{
  Icontainer con1(array1, array1 + 6);
  Ocontainer con2(array2, array2 + 2);
  VERIFY( unique_copy(con1.begin(), con1.end(), con2.begin()).ptr 
	  == array2 + 2 );
  VERIFY( array2[0] == 0 && array2[1] == 1 );
}

void
test3()
{
  Icontainer con1(array1, array1);
  Fcontainer con2(array2, array2);
  VERIFY( unique_copy(con1.begin(), con1.end(), con2.begin()).ptr == array2 );
}

void
test4()
{
  Icontainer con1(array1, array1 + 6);
  Fcontainer con2(array2, array2 + 2);
  VERIFY( unique_copy(con1.begin(), con1.end(), con2.begin()).ptr
	  == array2 + 2 );
  VERIFY( array2[0] == 0 && array2[1] == 1 );
}

int 
main()
{
  test1();
  test2();
  test3();
  test4();
}
