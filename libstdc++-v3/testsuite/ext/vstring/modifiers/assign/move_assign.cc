// { dg-do run { target c++11 } }
// { dg-require-string-conversions "" }

// Copyright (C) 2010-2023 Free Software Foundation, Inc.
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

// NOTE: This makes use of the fact that we know how moveable
// is implemented on vstring (via swap). If the implementation changes
// this test may begin to fail.

#include <ext/vstring.h>
#include <utility>
#include <testsuite_hooks.h>

void test01()
{
  __gnu_cxx::__sso_string a, b;
  a.push_back('1');
  b.assign(std::move(a));
  VERIFY( b.size() == 1 && b[0] == '1' && a.size() == 0 );
}

void test02()
{
  __gnu_cxx::__rc_string a, b;
  a.push_back('1');
  b.assign(std::move(a));
  VERIFY( b.size() == 1 && b[0] == '1' && a.size() == 0 );
}

int main()
{
  test01();
  test02();
  return 0;
}

// The __versa_string destructor triggers a bogus -Wfree-nonheap-object
// due to pr54202.
// { dg-prune-output "\\\[-Wfree-nonheap-object" }

