// Copyright (C) 2017-2023 Free Software Foundation, Inc.
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

// { dg-do run { target c++11 } }

#include <codecvt>
#include <testsuite_hooks.h>

const int bomlen = 3; // UTF-8 BOM is 24 bits

void
test01()
{
  const int maxlen = 3;

  std::codecvt_utf8<char16_t> c;
  VERIFY( c.always_noconv() == false );
  VERIFY( c.encoding() == 0 );
  VERIFY( c.max_length() == maxlen );

  std::codecvt_utf8<char16_t, 0x10ffff, std::consume_header> c_bom;
  VERIFY( c_bom.always_noconv() == false );
  VERIFY( c_bom.encoding() == 0 );
  VERIFY( c_bom.max_length() == (maxlen + bomlen) );
}

void
test02()
{
  const int maxlen = 4;

  std::codecvt_utf8<char32_t> c;
  VERIFY( c.always_noconv() == false );
  VERIFY( c.encoding() == 0 );
  VERIFY( c.max_length() == maxlen );

  std::codecvt_utf8<char32_t, 0x10ffff, std::consume_header> c_bom;
  VERIFY( c_bom.always_noconv() == false );
  VERIFY( c_bom.encoding() == 0 );
  VERIFY( c_bom.max_length() == (maxlen + bomlen) );
}

void
test03()
{
#ifdef _GLIBCXX_USE_WCHAR_T
  const int maxlen = sizeof(wchar_t) == 4 ? 4 : 3;

  std::codecvt_utf8<wchar_t> c;
  VERIFY( c.always_noconv() == false );
  VERIFY( c.encoding() == 0 );
  VERIFY( c.max_length() == maxlen );

  std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header> c_bom;
  VERIFY( c_bom.always_noconv() == false );
  VERIFY( c_bom.encoding() == 0 );
  VERIFY( c_bom.max_length() == (maxlen + bomlen) );
#endif
}

int
main()
{
  test01();
  test02();
  test03();
}
