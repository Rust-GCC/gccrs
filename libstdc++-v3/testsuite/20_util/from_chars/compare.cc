// Copyright (C) 2020-2023 Free Software Foundation, Inc.
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

// { dg-options "-std=gnu++2a" }
// { dg-do run { target c++2a } }

#include <charconv>
#include <testsuite_hooks.h>

void
test01()
{
  std::from_chars_result r1{}, r2{};
  VERIFY( r1 == r1 );
  VERIFY( !(r1 != r1) );
  VERIFY( r2 == r1 );
  VERIFY( !(r2 != r1) );
  r1.ptr = "";
  VERIFY( r1 == r1 );
  VERIFY( r1 != r2 );
  r2.ptr = "a" + 1;
  VERIFY( r1 != r2 );
  r2.ptr = r1.ptr;
  r2.ec = std::errc::invalid_argument;
  VERIFY( r1 != r2 );
  r1.ec = std::errc::no_such_file_or_directory;
  VERIFY( r1 != r2 );
  r1.ec = std::errc::invalid_argument;
  VERIFY( r2 == r1 );
}

int
main()
{
  test01();
}
