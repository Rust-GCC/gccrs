// Copyright (C) 2019 Free Software Foundation, Inc.
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

// { dg-do run }

#include <algorithm>
#include <testsuite_hooks.h>

namespace gnu_test
{
  // This is the definition from GCC 8.x, with no checks for first==middle
  // or middle==last.
  template<typename _ForwardIterator>
    inline _ForwardIterator
    rotate(_ForwardIterator __first, _ForwardIterator __middle,
	   _ForwardIterator __last)
    {
      return std::__rotate(__first, __middle, __last,
			   std::__iterator_category(__first));
    }
}

void
test01()
{
  int i = 0;
  gnu_test::rotate(&i, &i, &i+1);
}

int
main()
{
  test01();
}
