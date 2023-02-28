// 981208 bkoz test functionality of basic_stringbuf for char_type == char

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

std::string str_01("mykonos. . . or what?");
std::stringbuf strb_01(str_01);

// test overloaded virtual functions
void test04() 
{
  std::string 		str_tmp;
  typedef std::stringbuf::int_type int_type;
  typedef std::stringbuf::pos_type pos_type;
  typedef std::stringbuf::off_type off_type;

  int_type c1 = strb_01.sbumpc();
  int_type c3 = strb_01.sbumpc();

  pos_type pt_1(off_type(-1));
  pos_type pt_2(off_type(0));
  off_type off_1 = 0;
  off_type off_2 = 0;
  
  // BUFFER MANAGEMENT & POSITIONING

  // seekpos
  // pubseekpos(pos_type sp, ios_base::openmode)
  // alters the stream position to sp
  strb_01.str(str_01); //in|out ("mykonos. . . or what?");

  //IN|OUT
  //beg
  pt_1 = strb_01.pubseekoff(2, std::ios_base::beg);
  off_1 = off_type(pt_1);
  VERIFY( off_1 >= 0 );
  pt_1 = strb_01.pubseekoff(0, std::ios_base::cur, std::ios_base::out);
  off_1 = off_type(pt_1);
  c1 = strb_01.snextc(); //current in pointer +1
  VERIFY( c1 == 'o' );
  strb_01.sputc('x');  //test current out pointer
  str_tmp = std::string("myxonos. . . or what?");
  VERIFY( strb_01.str() == str_tmp );
  strb_01.pubsync(); //resets pointers
  pt_2 = strb_01.pubseekpos(pt_1, std::ios_base::in|std::ios_base::out);
  off_2 = off_type(pt_2);
  VERIFY( off_1 == off_2 );
  c3 = strb_01.snextc(); //current in pointer +1
  VERIFY( c1 == c3 );
  strb_01.sputc('x');  //test current out pointer
  str_tmp = std::string("myxonos. . . or what?");
  VERIFY( strb_01.str() == str_tmp );
}

int main()
{
  test04();
  return 0;
}



// more candy!!!
