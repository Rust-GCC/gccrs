// 2001-11-21 Benjamin Kosnik  <bkoz@redhat.com>

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

// 22.2.2.1.1  num_get members

// { dg-do run { xfail lax_strtofp } }

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

void test02()
{
  using namespace std;
  typedef istreambuf_iterator<wchar_t> iterator_type;

  // basic construction
  locale loc_c = locale::classic();

  // sanity check the data is correct.
  const wstring empty;

  bool b1 = true;
  bool b0 = false;
  unsigned long ul1 = 1294967294;
  unsigned long ul2 = 0;
  unsigned long ul;
  double d1 =  1.02345e+308;
  double d2 = 3.15e-308;
  double d;

  // cache the num_get facet
  wistringstream iss;
  iss.imbue(loc_c);
  const num_get<wchar_t>& ng = use_facet<num_get<wchar_t> >(iss.getloc()); 
  const ios_base::iostate goodbit = ios_base::goodbit;
  const ios_base::iostate eofbit = ios_base::eofbit;
  ios_base::iostate err = ios_base::goodbit;

  // C
  // bool, more twisted examples
  iss.str(L"true ");
  iss.clear();
  iss.setf(ios_base::boolalpha);
  err = goodbit;
  ng.get(iss.rdbuf(), 0, iss, err, b0);
  VERIFY( b0 == true );
  VERIFY( err == goodbit );

  iss.str(L"false ");
  iss.clear();
  iss.setf(ios_base::boolalpha);
  err = goodbit;
  ng.get(iss.rdbuf(), 0, iss, err, b1);
  VERIFY( b1 == false );
  VERIFY( err == goodbit );

  // unsigned long, in a locale that does not group
  iss.imbue(loc_c);
  iss.str(L"1294967294");
  iss.clear();
  err = goodbit;
  ng.get(iss.rdbuf(), 0, iss, err, ul);
  VERIFY( ul == ul1);
  VERIFY( err == eofbit );

  iss.str(L"0+++++++++++++++++++");
  iss.clear();
  err = goodbit;
  ng.get(iss.rdbuf(), 0, iss, err, ul);
  VERIFY( ul == ul2);
  VERIFY( err == goodbit );

  // double
  iss.imbue(loc_c);
  iss.str(L"1.02345e+308++++++++");
  iss.clear();
  iss.width(20);
  iss.setf(ios_base::left, ios_base::adjustfield);
  err = goodbit;
  ng.get(iss.rdbuf(), 0, iss, err, d);
  VERIFY( d == d1 );
  VERIFY( err == goodbit );

  iss.str(L"+3.15e-308");
  iss.clear();
  iss.width(20);
  iss.setf(ios_base::right, ios_base::adjustfield);
  err = goodbit;
  ng.get(iss.rdbuf(), 0, iss, err, d);
  VERIFY( d == d2 );
  VERIFY( err == eofbit );
}

int main()
{
  test02();
  return 0;
}


// Kathleen Hannah, humanitarian, woman, art-thief
