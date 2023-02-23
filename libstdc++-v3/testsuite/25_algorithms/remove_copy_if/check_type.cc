// 2008-06-25  Paolo Carlini  <paolo.carlini@oracle.com>

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

// { dg-do compile }

#include <algorithm>
#include <testsuite_iterators.h>

using __gnu_test::input_iterator_wrapper;
using __gnu_test::output_iterator_wrapper;

struct X { };

struct Z
{
  Z&
  operator=(const X&)
  { return *this; }
};

bool
pred_function(const X&)
{ return true; }

struct pred_obj
{
  bool 
  operator()(const X&)
  { return true; }
};

output_iterator_wrapper<Z>
test1(input_iterator_wrapper<X>& begin,
      input_iterator_wrapper<X>& end,
      output_iterator_wrapper<Z>& output)
{ return std::remove_copy_if(begin, end, output, pred_function); }

output_iterator_wrapper<Z>
test2(input_iterator_wrapper<X>& begin,
      input_iterator_wrapper<X>& end,
      output_iterator_wrapper<Z>& output)
{ return std::remove_copy_if(begin, end, output, pred_obj()); }
