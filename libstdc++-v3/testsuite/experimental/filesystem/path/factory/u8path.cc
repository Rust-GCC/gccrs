// Copyright (C) 2019 Free Software Foundation, Inc.
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

// { dg-options "-lstdc++fs" }
// { dg-do run { target c++11 } }
// { dg-require-filesystem-ts "" }

#include <experimental/filesystem>
#include <testsuite_hooks.h>

namespace fs = std::experimental::filesystem;

void
test01()
{
  fs::path p = fs::u8path("");
  VERIFY( p.empty() );

  p = fs::u8path("filename");
  VERIFY( p.u8string() == u8"filename" );

  p = fs::u8path("\xf0\x9d\x84\x9e");
  VERIFY( p.u8string() == u8"\U0001D11E" );
}

void
test02()
{
  // These calls to u8path are undefined, because they fail to meet the
  // requirement that the input is valid UTF-8 data. For Windows u8path
  // will fail. For POSIX constructing an invalid path appears to work,
  // but will fail when converted to a different encoding.

  try {
    auto p = fs::u8path("\xf0\x9d"); // incomplete surrogate pair
    p.u16string();
    VERIFY( false );
  } catch(const fs::filesystem_error&) {
  }

  try {
    auto p = fs::u8path("\xf0"); // incomplete multibyte character
    p.u16string();
    VERIFY( false );
  } catch(const fs::filesystem_error&) {
  }
}

int
main()
{
  test01();
  test02();
}
