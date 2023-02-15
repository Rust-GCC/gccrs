// 2001-05-24 Benjamin Kosnik  <bkoz@redhat.com>

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

// 27.7.6 member functions (stringstream_members)

#include <sstream>
#include <testsuite_hooks.h>

// libstdc++/8466
void test04()
{
  const char* strlit00 = "orvieto";
  const std::string str00 = strlit00;

  std::ostringstream oss;

  oss.str(str00);
  oss << "cortona";
  VERIFY( str00 == strlit00 );
}

int main()
{
  test04();
  return 0;
}
