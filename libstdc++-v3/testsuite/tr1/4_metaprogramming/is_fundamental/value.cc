// 2004-12-03  Paolo Carlini  <pcarlini@suse.de>
//
// Copyright (C) 2004-2023 Free Software Foundation, Inc.
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

// 4.5.2 Composite type traits

#include <tr1/type_traits>
#include <testsuite_hooks.h>
#include <testsuite_tr1.h>

void test01()
{
  using std::tr1::is_fundamental;
  using namespace __gnu_test;

  VERIFY( (test_category<is_fundamental, void>(true)) );
  VERIFY( (test_category<is_fundamental, char>(true)) );
  VERIFY( (test_category<is_fundamental, signed char>(true)) );
  VERIFY( (test_category<is_fundamental, unsigned char>(true)) );
  VERIFY( (test_category<is_fundamental, wchar_t>(true)) );
  VERIFY( (test_category<is_fundamental, short>(true)) );
  VERIFY( (test_category<is_fundamental, unsigned short>(true)) );
  VERIFY( (test_category<is_fundamental, int>(true)) );
  VERIFY( (test_category<is_fundamental, unsigned int>(true)) );
  VERIFY( (test_category<is_fundamental, long>(true)) );
  VERIFY( (test_category<is_fundamental, unsigned long>(true)) );
  VERIFY( (test_category<is_fundamental, long long>(true)) );
  VERIFY( (test_category<is_fundamental, unsigned long long>(true)) );
  VERIFY( (test_category<is_fundamental, float>(true)) );
  VERIFY( (test_category<is_fundamental, double>(true)) );
  VERIFY( (test_category<is_fundamental, long double>(true)) );

  // Sanity check.
  VERIFY( (test_category<is_fundamental, ClassType>(false)) );
}

int main()
{
  test01();
  return 0;
}
