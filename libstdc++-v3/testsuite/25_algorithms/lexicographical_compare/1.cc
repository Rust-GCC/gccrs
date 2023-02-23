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

// 25.3.8 [lib.alg.lex.comparison]

#include <algorithm>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>

using __gnu_test::test_container;
using __gnu_test::input_iterator_wrapper;

typedef test_container<int, input_iterator_wrapper> Container;
int array1[] = {0, 1};
int array2[] = {1, 0};
int array3[] = {1, 0, 1};

void
test1()
{
  Container con1(array1, array1);
  Container con2(array2, array2);
  VERIFY( !std::lexicographical_compare(con1.begin(), con1.end(),
					con2.begin(), con2.end()) );
}

void
test2()
{
  Container con1(array1, array1 + 2);
  Container con2(array2, array2 + 2);
  VERIFY( std::lexicographical_compare(con1.begin(), con1.end(),
				       con2.begin(), con2.end()) );
}

void
test3()
{
  Container con1(array1, array1 + 2);
  Container con2(array2, array2 + 2);
  VERIFY( !std::lexicographical_compare(con2.begin(), con2.end(),
				        con1.begin(), con1.end()) );
}

void
test4()
{
  Container con3(array3, array3 + 3);
  Container con2(array2, array2 + 2);
  VERIFY( std::lexicographical_compare(con2.begin(), con2.end(),
				       con3.begin(), con3.end()) );
}

void
test5()
{
  Container con3(array3, array3 + 3);
  Container con2(array2, array2 + 2);
  VERIFY( !std::lexicographical_compare(con3.begin(), con3.end(),
					con2.begin(), con2.end()) );
}

void
test6()
{
  VERIFY( std::lexicographical_compare(array2, array2 + 2,
				       array3, array3 + 3) );
  VERIFY( !std::lexicographical_compare(array3, array3 + 3,
					array2, array2 + 2) );
}

using __gnu_test::random_access_iterator_wrapper;
typedef test_container<int, random_access_iterator_wrapper> RaiContainer;

void
test7()
{
  RaiContainer con2(array2, array2 + 2);
  RaiContainer con3(array3, array3 + 3);
  VERIFY( std::lexicographical_compare(con2.begin(), con2.end(),
				       con3.begin(), con3.end()) );
  VERIFY( !std::lexicographical_compare(con3.begin(), con3.end(),
					con2.begin(), con2.end()) );
}

int
main()
{
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
}
