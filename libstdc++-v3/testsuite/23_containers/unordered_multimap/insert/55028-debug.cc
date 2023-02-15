// { dg-do run { target c++11 } }
//
// Copyright (C) 2012-2023 Free Software Foundation, Inc.
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

// libstdc++/55028
#include <string>
#include <unordered_map>
#include <testsuite_hooks.h>

struct MyType
{ };

void test()
{
  // using MyMap = std::multimap<std::string, MyType *>; // works
  using MyMap = std::unordered_multimap<std::string, MyType*>; // fails to link
  MyMap m;
  m.insert(std::make_pair(std::string("blah"), (MyType*)nullptr));
}

int main()
{
  test();
  return 0;
}
