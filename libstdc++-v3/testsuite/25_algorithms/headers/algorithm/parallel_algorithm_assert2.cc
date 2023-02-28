// { dg-do run }
// { dg-require-parallel-mode "" }
// { dg-options "-fopenmp -D_GLIBCXX_PARALLEL" { target *-*-* } }

// Copyright (C) 2016-2023 Free Software Foundation, Inc.
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

#undef _GLIBCXX_DEBUG
#undef _GLIBCXX_ASSERTIONS
#undef _GLIBCXX_PARALLEL_ASSERTIONS
#define _GLIBCXX_PARALLEL_ASSERTIONS 1
#include <parallel/algorithm>
#include <testsuite_hooks.h>

void
test01()
{
  // This should not be enabled without _GLIBCXX_ASSERTIONS:
  __glibcxx_assert(false);
}

void
test02()
{
  bool result = false;

  // This should be enabled by _GLIBCXX_PARALLEL_ASSERTIONS:
  _GLIBCXX_PARALLEL_ASSERT(result = true);
  VERIFY(result);
}

int main()
{
  test01();
  test02();
}
