// 2003-12-22  Paolo Carlini  <pcarlini@suse.de>

// Copyright (C) 2003-2023 Free Software Foundation, Inc.
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

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

struct Punct1: std::numpunct<wchar_t>
{
  std::string do_grouping() const { return "\1"; }
  wchar_t do_thousands_sep() const { return L'+'; }
  wchar_t do_decimal_point() const { return L'x'; }
};

struct Punct2: std::numpunct<wchar_t>
{
  std::string do_grouping() const { return "\1"; }
  wchar_t do_thousands_sep() const { return L'X'; }
  wchar_t do_decimal_point() const { return L'-'; }
};

// http://gcc.gnu.org/ml/libstdc++/2003-12/msg00201.html
void test01()
{
  using namespace std;
  typedef istreambuf_iterator<wchar_t> iterator_type;

  wistringstream iss1, iss2;
  iss1.imbue(locale(iss1.getloc(), new Punct1));
  iss2.imbue(locale(iss2.getloc(), new Punct2));
  const num_get<wchar_t>& ng1 = use_facet<num_get<wchar_t> >(iss1.getloc()); 
  const num_get<wchar_t>& ng2 = use_facet<num_get<wchar_t> >(iss2.getloc()); 

  ios_base::iostate err = ios_base::goodbit;
  iterator_type end;
  long l = 1l;
  long l1 = 0l;
  long l2 = 10l;
  long l3 = 1l;
  long l4 = 63l;
  double d = 0.0;
  double d1 = .4;
  double d2 = 0.0;
  double d3 = .1;

  iss1.str(L"+3");
  err = ios_base::goodbit;
  end = ng1.get(iss1.rdbuf(), 0, iss1, err, l);
  VERIFY( err == ios_base::failbit );
  VERIFY( *end == L'+' );

  iss1.str(L"0x1");
  iss1.clear();
  err = ios_base::goodbit;
  end = ng1.get(iss1.rdbuf(), 0, iss1, err, l);
  VERIFY( err == ios_base::goodbit );
  VERIFY( *end == L'x' );
  VERIFY( l == l1 );

  iss1.str(L"0Xa");
  iss1.clear();
  iss1.unsetf(ios::basefield);
  err = ios_base::goodbit;
  end = ng1.get(iss1.rdbuf(), 0, iss1, err, l);
  VERIFY( err == ios_base::eofbit );
  VERIFY( l == l2 );

  iss1.str(L"0xa");
  iss1.clear();
  err = ios_base::goodbit;
  end = ng1.get(iss1.rdbuf(), 0, iss1, err, l);
  VERIFY( err == ios_base::goodbit );
  VERIFY( *end == L'x' );
  VERIFY( l == l1 );  

  iss1.str(L"+5");
  iss1.clear();
  err = ios_base::goodbit;
  end = ng1.get(iss1.rdbuf(), 0, iss1, err, d);
  VERIFY( err == ios_base::failbit );
  VERIFY( *end == L'+' );

  iss1.str(L"x4");
  iss1.clear();
  err = ios_base::goodbit;
  end = ng1.get(iss1.rdbuf(), 0, iss1, err, d);
  VERIFY( err == ios_base::eofbit );
  VERIFY( d == d1 );

  iss2.str(L"0001-");
  err = ios_base::goodbit;
  end = ng2.get(iss2.rdbuf(), 0, iss2, err, l);
  VERIFY( err == ios_base::goodbit );
  VERIFY( *end == L'-' );
  VERIFY( l == l3 );

  iss2.str(L"-2");
  iss2.clear();
  err = ios_base::goodbit;
  end = ng2.get(iss2.rdbuf(), 0, iss2, err, l);
  VERIFY( err == ios_base::failbit );
  VERIFY( *end == L'-' );

  iss2.str(L"0X1");
  iss2.clear();
  iss2.unsetf(ios::basefield);
  err = ios_base::goodbit;
  end = ng2.get(iss2.rdbuf(), 0, iss2, err, l);
  VERIFY( err == ios_base::failbit );
  VERIFY( *end == L'X' );
  VERIFY( l == 0 );

  iss2.str(L"000778");
  iss2.clear();
  err = ios_base::goodbit;
  end = ng2.get(iss2.rdbuf(), 0, iss2, err, l);
  VERIFY( err == ios_base::goodbit );
  VERIFY( *end == L'8' );
  VERIFY( l == l4 );

  iss2.str(L"00X");
  iss2.clear();
  err = ios_base::goodbit;
  end = ng2.get(iss2.rdbuf(), 0, iss2, err, d);
  VERIFY( err == (ios_base::eofbit | ios_base::failbit) );
  VERIFY( d == d2 );

  iss2.str(L"-1");
  iss2.clear();
  err = ios_base::goodbit;
  end = ng2.get(iss2.rdbuf(), 0, iss2, err, d);
  VERIFY( err == ios_base::eofbit );
  VERIFY( d == d3 );  
}

int main()
{
  test01();
  return 0;
}
