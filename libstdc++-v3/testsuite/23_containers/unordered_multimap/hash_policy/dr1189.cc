// { dg-do run { target c++11 } }
// 2010-03-10  Paolo Carlini  <paolo.carlini@oracle.com>
//
// Copyright (C) 2010-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <unordered_map>
#include <testsuite_hooks.h>

// DR 1189. Awkward interface for changing the number of buckets
// in an unordered associative container
void test01() 
{
  std::unordered_multimap<int, double> mm1;
  mm1.reserve(10);
  VERIFY( mm1.bucket_count() >= 10 );

  mm1.reserve(100);
  VERIFY( mm1.bucket_count() >= 100 );

  std::unordered_map<int, double> mm2(100);
  VERIFY( mm2.bucket_count() >= 100 );

  mm2.reserve(1000);
  VERIFY( mm2.bucket_count() >= 1000 );
}

int main()
{
  test01();
  return 0;
}
