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

// { dg-require-fileio "" }

#include <iterator>
#include <fstream>
#include <algorithm>

#include <testsuite_hooks.h>

void test01()
{
  using namespace std;

  typedef istreambuf_iterator<wchar_t> in_iterator_type;

  unsigned found = 0;

  {
    wifstream fbuf("istream_unformatted-1.txt");

    in_iterator_type beg(fbuf);
    in_iterator_type end;

    for (;;)
      {
	beg = find(beg, end, L'1');
	if (beg == end)
	  break;

	++found;
	VERIFY( *beg == L'1' );

	advance(beg, 9);
	VERIFY( *beg == L'0' );
      }
  }

  {
    wifstream fbuf("istream_unformatted-1.txt");

    in_iterator_type beg(fbuf);
    in_iterator_type end;

    beg = find(beg, end, L'1');
    VERIFY( beg != end );
    VERIFY( *beg == L'1' );

    advance(beg, 9);
    VERIFY( *beg == L'0' );

    unsigned line_length = 10;
    while (*++beg != L'1')
      ++line_length;

    // Try to jump directly to the end through advance.
    advance(beg, (found - 2) * line_length + 9);
    VERIFY( *beg == L'0' );
    VERIFY( find(beg, end, L'1') == end );
  }
}

int main()
{
  test01();
  return 0;
}
