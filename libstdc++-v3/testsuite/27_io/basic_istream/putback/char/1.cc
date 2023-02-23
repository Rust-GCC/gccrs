// 1999-08-11 bkoz

// Copyright (C) 1999-2023 Free Software Foundation, Inc.
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
test01()
{
  const std::string str_01;
  const std::string str_02("soul eyes: john coltrane quartet");
  std::string strtmp;

  std::stringbuf isbuf_03(str_02, std::ios_base::in);
  std::stringbuf isbuf_04(str_02, std::ios_base::in);
  std::stringbuf isbuf_05(str_02, std::ios_base::in);

  std::istream is_00(&isbuf_05);
  std::istream is_03(&isbuf_03);
  std::istream is_04(&isbuf_04);
  std::ios_base::iostate state1, state2;

  // istream& putback(char c)
  is_04.ignore(30);
  is_04.clear();
  state1 = is_04.rdstate();
  is_04.putback('t');
  VERIFY( is_04.gcount() == 0 );  // DR 60
  state2 = is_04.rdstate();
  VERIFY( state1 == state2 );
  VERIFY( is_04.peek() == 't' );

  // istream& unget()
  is_04.clear();
  state1 = is_04.rdstate();
  is_04.unget();
  VERIFY( is_04.gcount() == 0 );  // DR 60
  state2 = is_04.rdstate();
  VERIFY( state1 == state2 );
  VERIFY( is_04.peek() == 'r' );
  
  // int sync()
  is_00.ignore(10);
  int count1 = is_00.gcount();
  is_00.sync();
  int count2 = is_00.gcount();
  VERIFY (count1 == count2 );     // DR 60
}

int 
main()
{
  test01();
  return 0;
}
