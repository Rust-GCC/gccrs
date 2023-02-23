// 2006-10-12  Paolo Carlini  <pcarlini@suse.de>

// Copyright (C) 2006-2023 Free Software Foundation, Inc.
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

// { dg-options "-DWIDTH=200000" { target simulator } }

// 21.3.7.9 inserters and extractors

#include <ostream>
#include <sstream>
#include <testsuite_hooks.h>

#ifndef WIDTH
#define WIDTH 20000000
#endif

// libstdc++/28277
void test01()
{
  using namespace std;

  ostringstream oss_01;
  const string str_01(50, 'a');

  oss_01.width(WIDTH);
  const streamsize width = oss_01.width();

  oss_01 << str_01;

  VERIFY( oss_01.good() );
  VERIFY( oss_01.str().size() == string::size_type(width) );
}

int main()
{
  test01();
  return 0;
}
