// { dg-require-namedlocale "en_HK.ISO8859-1" }

// 2004-03-15  Paolo Carlini  <pcarlini@suse.de>

// Copyright (C) 2004-2023 Free Software Foundation, Inc.
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

// 22.2.6.1.1 money_get members

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

// If (str.flags() & str.showbase) is false, the currency symbol is optional,
// but, if found, must be consumed entirely.
void test01()
{
  using namespace std;
  typedef istreambuf_iterator<wchar_t> iterator_type;

  // basic construction
  locale loc_c = locale::classic();
  locale loc_hk = locale(ISO_8859(1,en_HK));
  VERIFY( loc_c != loc_hk );
  
  iterator_type end, end01, end02;
  wistringstream iss;
  iss.imbue(loc_hk);
  // cache the money_get facet
  const money_get<wchar_t>& mon_get =
    use_facet<money_get<wchar_t> >(iss.getloc()); 

  iss.str(L"HK7,200,000,000.00"); 
  iterator_type is_it01(iss);
  wstring result01;
  ios_base::iostate err01 = ios_base::goodbit;
  end01 = mon_get.get(is_it01, end, false, iss, err01, result01);
  VERIFY( err01 == ios_base::failbit );
  VERIFY( *end01 == L'7' );

  iss.str(L"(HK100,000,000,000.00)"); 
  iterator_type is_it02(iss);
  wstring result02;
  ios_base::iostate err02 = ios_base::goodbit;
  end02 = mon_get.get(is_it02, end, true, iss, err02, result02);
  VERIFY( err02 == ios_base::failbit );
  VERIFY( *end02 == L'1' );
}

int main()
{
  test01();
  return 0;
}
