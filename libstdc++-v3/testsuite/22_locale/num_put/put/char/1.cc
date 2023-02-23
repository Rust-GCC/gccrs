// { dg-require-namedlocale "de_DE.ISO8859-15" }

// 2001-11-19 Benjamin Kosnik  <bkoz@redhat.com>

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

// 22.2.2.2.1  num_put members

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

void test01()
{
  using namespace std;
  typedef ostreambuf_iterator<char> iterator_type;

  // basic construction
  locale loc_c = locale::classic();
  locale loc_de  = locale(ISO_8859(15,de_DE));
  VERIFY( loc_c != loc_de );

  // cache the numpunct facets
  const numpunct<char>& numpunct_de = use_facet<numpunct<char> >(loc_de); 

  // sanity check the data is correct.
  const string empty;
  string result1;
  string result2;

  bool b1 = true;
  bool b0 = false;
  unsigned long ul1 = 1294967294;
  double d1 =  1.7976931348623157e+308;
  double d2 = 2.2250738585072014e-308;
  long double ld1 = 1.7976931348623157e+308;
  long double ld2 = 2.2250738585072014e-308;
  const void* cv = &ld1;

  // cache the num_put facet
  ostringstream oss;
  oss.imbue(loc_de);
  const num_put<char>& np = use_facet<num_put<char> >(oss.getloc()); 

  // bool, simple
  iterator_type os_it00 = oss.rdbuf();
  np.put(os_it00, oss, '+', b1);
  result1 = oss.str();
  VERIFY( result1 == "1" );

  oss.str(empty);
  np.put(oss.rdbuf(), oss, '+', b0);
  result2 = oss.str();
  VERIFY( result2 == "0" );

  // ... and one that does
  oss.imbue(loc_de);
  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, '+', ul1);
  result1 = oss.str();
  VERIFY( result1 == "1.294.967.294+++++++" );

  // double
  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, '+', d1);
  result1 = oss.str();
  VERIFY( result1 == "1,79769e+308++++++++" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::right, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, '+', d2);
  result1 = oss.str();
  VERIFY( result1 == "++++++++2,22507e-308" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::right, ios_base::adjustfield);
  oss.setf(ios_base::scientific, ios_base::floatfield);
  np.put(oss.rdbuf(), oss, '+', d2);
  result2 = oss.str();
  VERIFY( result2 == "+++++++2,225074e-308" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.precision(10);
  oss.setf(ios_base::right, ios_base::adjustfield);
  oss.setf(ios_base::scientific, ios_base::floatfield);
  oss.setf(ios_base::uppercase);
  np.put(oss.rdbuf(), oss, '+', d2);
  result1 = oss.str();
  VERIFY( result1 == "+++2,2250738585E-308" );

  // long double
  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, '+', ld1);
  result1 = oss.str();
  VERIFY( result1 == "1,7976931349E+308" );

  oss.str(empty);
  oss.clear();
  oss.precision(0);
  oss.setf(ios_base::fixed, ios_base::floatfield);
  np.put(oss.rdbuf(), oss, '+', ld2);
  result1 = oss.str();
  VERIFY( result1 == "0" );

  // const void*
  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, '+', cv);
  result1 = oss.str();
  // No grouping characters.
  VERIFY( !char_traits<char>::find(result1.c_str(), 
				   result1.size(),
				   numpunct_de.decimal_point()) );
  // Should contain an 'x'.
  VERIFY( result1.find('x') == 1 );

#ifdef _GLIBCXX_USE_LONG_LONG
  long long ll1 = 9223372036854775807LL;

  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, '+', ll1);
  result1 = oss.str();
  VERIFY( result1 == "9.223.372.036.854.775.807" );
#endif
}

int main()
{
  test01();
  return 0;
}


