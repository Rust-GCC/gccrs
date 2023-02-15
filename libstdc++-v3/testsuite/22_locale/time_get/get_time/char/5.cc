// 2006-06-16  Paolo Carlini  <pcarlini@suse.de>

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

// 22.2.5.1.1 time_get members

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

// Check that the err argument is ignored by get_time.
void test01()
{
  using namespace std;

  typedef string::const_iterator iter_type;
  typedef time_get<char, iter_type> time_get_type;
  const ios_base::iostate goodbit = ios_base::goodbit;
  const ios_base::iostate eofbit = ios_base::eofbit;  
  const ios_base::iostate failbit = ios_base::failbit;
  ios_base::iostate err = goodbit;
  const locale loc_c = locale::classic();

  // Create "C" time objects
  const tm time_sanity = __gnu_test::test_tm(0, 0, 12, 26, 5, 97, 2, 0, 0);
  tm tm0 = __gnu_test::test_tm(0, 0, 0, 0, 0, 0, 0, 0, 0);
  tm tm1 = __gnu_test::test_tm(0, 0, 0, 0, 0, 0, 0, 0, 0);

  istringstream iss; 
  iss.imbue(locale(loc_c, new time_get_type));

  // Iterator advanced, state, output.
  const time_get_type& tg = use_facet<time_get_type>(iss.getloc());

  const string str0 = "1";
  tg.get_time(str0.begin(), str0.end(), iss, err, &tm0);
  VERIFY( err == (failbit | eofbit) );
  VERIFY( tm0.tm_sec == 0 );
  VERIFY( tm0.tm_min == 0 );
  VERIFY( tm0.tm_hour == 1 );

  const string str1 = "12:00:00 ";
  iter_type end1 = tg.get_time(str1.begin(), str1.end(), iss, err, &tm1);
  VERIFY( err == (failbit | eofbit) );
  VERIFY( tm1.tm_sec == time_sanity.tm_sec );
  VERIFY( tm1.tm_min == time_sanity.tm_min );
  VERIFY( tm1.tm_hour == time_sanity.tm_hour );
  VERIFY( *end1 == ' ' );
}

int main()
{
  test01();
  return 0;
}
