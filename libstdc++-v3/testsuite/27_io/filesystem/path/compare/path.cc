// { dg-do run { target c++17 } }

// Copyright (C) 2014-2023 Free Software Foundation, Inc.
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

// C++17 30.10.8.4.8 path compare [fs.path.compare]

#include <filesystem>
#include <testsuite_hooks.h>
#include <testsuite_fs.h>

using std::filesystem::path;

void
test01()
{
  const path p0 = "/a/a/b/b";
  for (const path p : __gnu_test::test_paths)
  {
    VERIFY( p.compare(p) == 0 );
    int cmp = p.compare(p0);
    if (cmp == 0)
      VERIFY( p0.compare(p) == 0 );
    else if (cmp < 0)
      VERIFY( p0.compare(p) > 0 );
    else if (cmp > 0)
      VERIFY( p0.compare(p) < 0 );
  }
}

void
test02()
{
  VERIFY( path("/").compare(path("////")) == 0 );
  VERIFY( path("/a").compare(path("/")) > 0 );
  VERIFY( path("/").compare(path("/a")) < 0 );
  VERIFY( path("/ab").compare(path("/a")) > 0 );
  VERIFY( path("/ab").compare(path("/a/b")) > 0 );
}

int
main()
{
  test01();
  test02();
}
