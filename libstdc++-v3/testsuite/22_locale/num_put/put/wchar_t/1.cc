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
  typedef ostreambuf_iterator<wchar_t> iterator_type;

  // basic construction
  locale loc_c = locale::classic();
  locale loc_de = locale(ISO_8859(15,de_DE));
  VERIFY( loc_c != loc_de );

  // cache the numpunct facets
  const numpunct<wchar_t>& numpunct_de = use_facet<numpunct<wchar_t> >(loc_de);
 
  // sanity check the data is correct.
  const wstring empty;
  wstring result1;
  wstring result2;

  bool b1 = true;
  bool b0 = false;
  unsigned long ul1 = 1294967294;
  double d1 =  1.7976931348623157e+308;
  double d2 = 2.2250738585072014e-308;
  long double ld1 = 1.7976931348623157e+308;
  long double ld2 = 2.2250738585072014e-308;
  const void* cv = &ld1;

  // cache the num_put facet
  wostringstream oss;
  oss.imbue(loc_de);
  const num_put<wchar_t>& np = use_facet<num_put<wchar_t> >(oss.getloc()); 

  // bool, simple
  iterator_type os_it00 = oss.rdbuf();
  np.put(os_it00, oss, L'+', b1);
  result1 = oss.str();
  VERIFY( result1 == L"1" );

  oss.str(empty);
  np.put(oss.rdbuf(), oss, L'+', b0);
  result2 = oss.str();
  VERIFY( result2 == L"0" );

  // ... and one that does
  oss.imbue(loc_de);
  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', ul1);
  result1 = oss.str();
  VERIFY( result1 == L"1.294.967.294+++++++" );

  // double
  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', d1);
  result1 = oss.str();
  VERIFY( result1 == L"1,79769e+308++++++++" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::right, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', d2);
  result1 = oss.str();
  VERIFY( result1 == L"++++++++2,22507e-308" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::right, ios_base::adjustfield);
  oss.setf(ios_base::scientific, ios_base::floatfield);
  np.put(oss.rdbuf(), oss, L'+', d2);
  result2 = oss.str();
  VERIFY( result2 == L"+++++++2,225074e-308" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.precision(10);
  oss.setf(ios_base::right, ios_base::adjustfield);
  oss.setf(ios_base::scientific, ios_base::floatfield);
  oss.setf(ios_base::uppercase);
  np.put(oss.rdbuf(), oss, L'+', d2);
  result1 = oss.str();
  VERIFY( result1 == L"+++2,2250738585E-308" );

  // long double
  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, L'+', ld1);
  result1 = oss.str();
  VERIFY( result1 == L"1,7976931349E+308" );

  oss.str(empty);
  oss.clear();
  oss.precision(0);
  oss.setf(ios_base::fixed, ios_base::floatfield);
  np.put(oss.rdbuf(), oss, L'+', ld2);
  result1 = oss.str();
  VERIFY( result1 == L"0" );

  // const void*
  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, L'+', cv);
  result1 = oss.str();
  // No grouping characters.
  VERIFY( !char_traits<wchar_t>::find(result1.c_str(), 
				   result1.size(),
				   numpunct_de.decimal_point()) );
  // Should contain an 'x'.
  VERIFY( result1.find(L'x') == 1 );

#ifdef _GLIBCXX_USE_LONG_LONG
  long long ll1 = 9223372036854775807LL;

  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, L'+', ll1);
  result1 = oss.str();
  VERIFY( result1 == L"9.223.372.036.854.775.807" );
#endif
}

int main()
{
  test01();
  return 0;
}
