// 981208 bkoz test functionality of basic_stringbuf for char_type == wchar_t

// Copyright (C) 1997-2023 Free Software Foundation, Inc.
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

#include <sstream>
#include <testsuite_hooks.h>

std::wstring str_01(L"mykonos. . . or what?");
std::wstring str_02(L"paris, or sainte-maxime?");
std::wstring str_03;
std::wstringbuf strb_01(str_01);
std::wstringbuf strb_02(str_02, std::ios_base::in);
std::wstringbuf strb_03(str_03, std::ios_base::out);

// test overloaded virtual functions
void test04() 
{
  std::wstring 		str_tmp;
  std::streamsize 		strmsz_1, strmsz_2;
  typedef std::wstringbuf::int_type int_type;
  typedef std::wstringbuf::traits_type traits_type;

  // PUT
  strb_03.str(str_01); //reset
  std::wstring::size_type sz1 = strb_03.str().length();
  std::wstring::size_type sz2 = strb_03.str().length();
  
  // streamsize sputn(const char_type* s, streamsize n)
  // write up to n chars to out_cur from s, returning number assigned
  // NB *sputn will happily put '\0' into your stream if you give it a chance*
  str_tmp = strb_03.str();
  sz1 = str_tmp.length();
  strmsz_1 = strb_03.sputn(L"racadabras", 10);//"abracadabras or what?"
  sz2 = strb_03.str().length();
  VERIFY( sz1 == sz2 ); //shouldn't have changed length
  VERIFY( strmsz_1 == 10 );
  VERIFY( str_tmp != strb_03.str() );
  strmsz_2 = strb_03.sputn(L", i wanna reach out and", 23);
  VERIFY( strmsz_1 != strmsz_2 ); // should re-allocate, copy 10 chars.
  VERIFY( strmsz_1 == 10 );
  VERIFY( strmsz_2 == 23 );
  sz2 = strb_03.str().length();
  VERIFY( sz1 != sz2 ); // need to change length
  VERIFY( str_tmp != strb_03.str() );
  str_tmp = strb_02.str();
  strmsz_1 = strb_02.sputn(L"racadabra", 10);
  VERIFY( strmsz_1 == 0 );  
  VERIFY( str_tmp == strb_02.str() );
}

int main()
{
  test04();
  return 0;
}



// more candy!!!
