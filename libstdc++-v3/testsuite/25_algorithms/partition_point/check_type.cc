// 2008-06-28  Paolo Carlini  <paolo.carlini@oracle.com>

// Copyright (C) 2008-2023 Free Software Foundation, Inc.
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

// { dg-do compile { target c++11 } }

#include <algorithm>
#include <testsuite_iterators.h>

struct X { };

using __gnu_test::forward_iterator_wrapper;

bool
pred_function(const X&)
{ return true; }

struct pred_obj
{
  bool 
  operator()(const X&)
  { return true; }
};

forward_iterator_wrapper<X>
test1(forward_iterator_wrapper<X>& begin,
      forward_iterator_wrapper<X>& end)
{ return std::partition_point(begin, end, pred_function); }

forward_iterator_wrapper<X>
test2(forward_iterator_wrapper<X>& begin,
      forward_iterator_wrapper<X>& end)
{ return std::partition_point(begin, end, pred_obj()); }
