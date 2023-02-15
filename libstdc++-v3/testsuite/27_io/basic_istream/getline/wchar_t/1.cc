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

// 27.6.1.3 unformatted input functions

#include <istream>
#include <sstream>
#include <testsuite_hooks.h>

void
test02()
{
  typedef std::char_traits<wchar_t>	traits_type;

  const wchar_t str_lit01[] = L"\t\t\t    sun*ra \n"
  L"                            "
  L"and his myth science arkestra present\n"
  L"                            "
  L"angles and demons @ play\n"
  L"                            "
  L"the nubians of plutonia";
  std::wstring str01(str_lit01);
  std::wstring strtmp;

  std::wstringbuf sbuf_04(str01, std::ios_base::in);

  std::wistream is_00(0);
  std::wistream is_04(&sbuf_04);
  std::ios_base::iostate state1, state2, statefail, stateeof;
  statefail = std::ios_base::failbit;
  stateeof = std::ios_base::eofbit;
  wchar_t carray1[400] = L"";

  // istream& getline(wchar_t* s, streamsize n, wchar_t delim)
  // istream& getline(wchar_t* s, streamsize n)
  state1 = is_00.rdstate();
  is_00.getline(carray1, 20, L'*');
  state2 = is_00.rdstate();
  // make sure failbit was set, since we couldn't extract
  // from the null streambuf...
  VERIFY( state1 != state2 );
  VERIFY( static_cast<bool>(state2 & statefail) );
  
  VERIFY( is_04.gcount() == 0 );
  state1 = is_04.rdstate();
  is_04.getline(carray1, 1, L'\t'); // extracts, throws away
  state2 = is_04.rdstate();  
  VERIFY( is_04.gcount() == 1 );
  VERIFY( state1 == state2 );
  VERIFY( state1 == 0 );
  VERIFY( !traits_type::compare(L"", carray1, 1) );

  state1 = is_04.rdstate();
  is_04.getline(carray1, 20, L'*');
  state2 = is_04.rdstate();  
  VERIFY( is_04.gcount() == 10 );
  VERIFY( state1 == state2 );
  VERIFY( state1 == 0 );
  VERIFY( !traits_type::compare(L"\t\t    sun", carray1, 10) );

  state1 = is_04.rdstate();
  is_04.getline(carray1, 20);
  state2 = is_04.rdstate();  
  VERIFY( is_04.gcount() == 4 );
  VERIFY( state1 == state2 );
  VERIFY( state1 == 0 );
  VERIFY( !traits_type::compare(L"ra ", carray1, 4) );

  state1 = is_04.rdstate();
  is_04.getline(carray1, 65);
  state2 = is_04.rdstate();  
  VERIFY( is_04.gcount() == 64 );
  VERIFY( state1 != state2 );
  VERIFY( state2 == statefail );
  VERIFY( !traits_type::compare(
  L"                            and his myth science arkestra presen",
                                carray1, 65) );

  is_04.clear();
  state1 = is_04.rdstate();
  is_04.getline(carray1, 120, L'|');
  state2 = is_04.rdstate();  
  VERIFY( is_04.gcount() == 106 );
  VERIFY( state1 != state2 );
  VERIFY( state2 == stateeof );

  is_04.clear();
  state1 = is_04.rdstate();
  is_04.getline(carray1, 100, L'|');
  state2 = is_04.rdstate();  
  VERIFY( is_04.gcount() == 0 ); 
  VERIFY( state1 != state2 );
  VERIFY( static_cast<bool>(state2 & stateeof) );
  VERIFY( static_cast<bool>(state2 & statefail) );
}
 
int 
main()
{
  test02();
  return 0;
}
