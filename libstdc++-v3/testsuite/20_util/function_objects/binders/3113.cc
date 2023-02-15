// 2001-06-11  Benjamin Kosnik  <bkoz@redhat.com>

// Copyright (C) 2001-2023 Free Software Foundation, Inc.
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

// { dg-options "-Wno-deprecated-declarations" }
// { dg-add-options using-deprecated }

// 20.3.6 Binders

#include <vector>
#include <algorithm> // for_each
#include <functional>

class Elem
{
public:
  void print(int) const { }
  void modify(int) { }
};

// libstdc++/3113
void test01()
{
  std::vector<Elem> coll(2);
  // OK
  std::for_each(coll.begin(), coll.end(),
	   std::bind2nd(std::mem_fun_ref(&Elem::print), 42));
  // OK
  std::for_each(coll.begin(), coll.end(),
	   std::bind2nd(std::mem_fun_ref(&Elem::modify), 42));
}

int main()
{
  test01();
  return 0;
}
