// 2001-02-26 Benjamin Kosnik  <bkoz@redhat.com>

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

// 19.1 Exception classes

#include <string>
#include <stdexcept>
#include <cstring>
#include <testsuite_hooks.h>

// libstdc++/1972
void test01()
{
  std::string s("lack of sunlight, no water error");

  // 1
  std::logic_error obj1 = std::logic_error(s);

  // 2
  std::logic_error obj2(s);

  VERIFY( std::strcmp(obj1.what(), s.data()) == 0 );
  VERIFY( std::strcmp(obj2.what(), s.data()) == 0 );
}

void test02()
{
  std::string s("lack of sunlight error");
  std::domain_error x(s);
  
  VERIFY( std::strcmp(x.what(), s.data()) == 0 );
}

int main(void)
{
  test01();
  test02();
  return 0;
}
