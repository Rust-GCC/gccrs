// 2005-01-17  Paolo Carlini  <pcarlini@suse.de>

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
//

#include <set>
#include <testsuite_hooks.h>

// A few tests for insert with hint, in the occasion of libstdc++/19422
// and libstdc++/19433.
void test01()
{
  using namespace std;

  set<int> s0, s1;
  set<int>::iterator iter1;
  
  s0.insert(1);
  s1.insert(s1.end(), 1);
  VERIFY( s0 == s1 );

  s0.insert(3);
  s1.insert(s1.begin(), 3);
  VERIFY( s0 == s1 );

  s0.insert(4);
  iter1 = s1.insert(s1.end(), 4);
  VERIFY( s0 == s1 );

  s0.insert(6);
  s1.insert(iter1, 6);
  VERIFY( s0 == s1 );

  s0.insert(2);
  s1.insert(s1.begin(), 2);
  VERIFY( s0 == s1 );

  s0.insert(7);
  s1.insert(s1.end(), 7);
  VERIFY( s0 == s1 );

  s0.insert(5);
  s1.insert(s1.find(4), 5);
  VERIFY( s0 == s1 );

  s0.insert(0);
  s1.insert(s1.end(), 0);
  VERIFY( s0 == s1 );

  s0.insert(8);
  s1.insert(s1.find(3), 8);
  VERIFY( s0 == s1 );
  
  s0.insert(9);
  s1.insert(s1.end(), 9);
  VERIFY( s0 == s1 );

  s0.insert(10);
  s1.insert(s1.begin(), 10);
  VERIFY( s0 == s1 );
}

int main ()
{
  test01();
  return 0;
}
