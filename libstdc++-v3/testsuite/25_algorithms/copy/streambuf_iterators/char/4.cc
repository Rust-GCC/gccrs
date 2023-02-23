// 2006-03-20  Paolo Carlini  <pcarlini@suse.de>

// Copyright (C) 2006-2023 Free Software Foundation, Inc.
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

#include <iterator>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <deque>

#include <testsuite_hooks.h>

// { dg-require-fileio "" }

// In the occasion of libstdc++/25482
void test01()
{
  using namespace std;

  typedef istreambuf_iterator<char> in_iterator_type;

  ifstream fbuf_ref("istream_unformatted-1.txt"),
	   fbuf("istream_unformatted-1.txt");

  char buffer_ref[16500],
       buffer[16500];

  fbuf_ref.read(buffer_ref, 16500);

  in_iterator_type beg(fbuf);
  in_iterator_type end;
  copy(beg, end, buffer);

  VERIFY( fbuf_ref.good() );
  VERIFY( fbuf.good() );

  VERIFY( !memcmp(buffer, buffer_ref, 16500) );
}

void test02()
{
  using namespace std;

  typedef istreambuf_iterator<char> in_iterator_type;

  ifstream fbuf_ref("istream_unformatted-1.txt"),
	   fbuf("istream_unformatted-1.txt");

  char buffer_ref[16500];
  std::vector<char> buffer(16500, 'a');

  fbuf_ref.read(buffer_ref, 16500);

  in_iterator_type beg(fbuf);
  in_iterator_type end;
  copy(beg, end, buffer.begin());

  VERIFY( fbuf_ref.good() );
  VERIFY( fbuf.good() );

  VERIFY( !memcmp(buffer.data(), buffer_ref, 16500) );
}

void test03()
{
  using namespace std;

  typedef istreambuf_iterator<char> in_iterator_type;

  ifstream fbuf_ref("istream_unformatted-1.txt"),
	   fbuf("istream_unformatted-1.txt");

  char buffer_ref[16500];
  std::deque<char> buffer(16500, 'a');

  fbuf_ref.read(buffer_ref, 16500);

  in_iterator_type beg(fbuf);
  in_iterator_type end;
  copy(beg, end, buffer.begin());

  VERIFY( fbuf_ref.good() );
  VERIFY( fbuf.good() );

  VERIFY( std::equal(buffer.begin(), buffer.end(), buffer_ref) );
}

int main()
{
  test01();
  test02();
  test03();
  return 0;
}
