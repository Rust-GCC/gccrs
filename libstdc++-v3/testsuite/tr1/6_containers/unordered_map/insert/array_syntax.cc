// { dg-do run }

// 2005-2-17  Matt Austern  <austern@apple.com>
//
// Copyright (C) 2005-2023 Free Software Foundation, Inc.
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

// 6.3.4.4 unordered_map
// Array version of insert

#include <string>
#include <iterator>
#include <tr1/unordered_map>
#include "testsuite_hooks.h"

void test01()
{
  typedef std::tr1::unordered_map<std::string, int> Map;
  typedef std::pair<const std::string, int> Pair;

  Map m;
  VERIFY(m.empty());

  m["red"] = 17;
  VERIFY(m.size() == 1);
  VERIFY(m.begin()->first == "red");
  VERIFY(m.begin()->second == 17);
  VERIFY(m["red"] == 17);

  m["blue"] = 9;
  VERIFY(m.size() == 2);
  VERIFY(m["blue"] == 9);

  m["red"] = 5;
  VERIFY(m.size() == 2);
  VERIFY(m["red"] == 5);
  VERIFY(m["blue"] == 9);
}

int main()
{
  test01();
  return 0;
}
