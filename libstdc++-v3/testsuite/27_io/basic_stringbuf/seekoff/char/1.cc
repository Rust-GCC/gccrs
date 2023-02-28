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
  std::streamsize 		strmsz_1, strmsz_2;
  typedef std::stringbuf::int_type int_type;
  typedef std::stringbuf::traits_type traits_type;
  typedef std::stringbuf::pos_type pos_type;
  typedef std::stringbuf::off_type off_type;

  int_type c1 = strb_01.sbumpc();
  int_type c2;

  // BUFFER MANAGEMENT & POSITIONING

  // seekoff
  // pubseekoff(off_type off, ios_base::seekdir way, ios_base::openmode which)
  // alters the stream position to off
  pos_type pt_1(off_type(-1));
  pos_type pt_2(off_type(0));
  off_type off_1 = 0;
  off_type off_2 = 0;
  strb_01.str(str_01); //in|out ("mykonos. . . or what?");

  //IN|OUT
  //beg
  pt_1 = strb_01.pubseekoff(2, std::ios_base::beg);
  off_1 = off_type(pt_1);
  VERIFY( off_1 >= 0 );
  c1 = strb_01.snextc(); //current in pointer +1
  VERIFY( c1 == 'o' );
  c2 = strb_01.sputc('x');  //test current out pointer
  str_tmp = std::string("myxonos. . . or what?");
  VERIFY( strb_01.str() == str_tmp );
  //cur
  pt_1 = strb_01.pubseekoff(2, std::ios_base::cur);
  off_1 = off_type(pt_1);
  VERIFY( off_1 == -1 ); // can't seekoff for in and out + cur in sstreams
  pt_1 = strb_01.pubseekoff(2, std::ios_base::cur, std::ios_base::in);
  off_1 = off_type(pt_1);
  pt_2 = strb_01.pubseekoff(2, std::ios_base::cur, std::ios_base::in);
  off_2 = off_type(pt_2);
  VERIFY( off_2 == off_1 + 2 );
  c1 = strb_01.snextc(); //current in pointer + 1
  VERIFY( c1 == ' ' );
  c2 = strb_01.sputc('x');  //test current out pointer
  str_tmp = std::string("myxxnos. . . or what?");
  VERIFY( strb_01.str() == str_tmp );
  //end
  pt_2 = strb_01.pubseekoff(2, std::ios_base::end);
  off_1 = off_type(pt_2);
  VERIFY( off_1 == -1 ); // not a valid position
  VERIFY( strb_01.str() == str_tmp );
  // end part two (from the filebuf tests)
  strb_01.pubseekoff(0, std::ios_base::end);
  strmsz_1 = strb_01.in_avail(); // 0 cuz at the end
  c1 = strb_01.sgetc(); 
  c2 = strb_01.sungetc();
  strmsz_2 = strb_01.in_avail(); // 1
  strb_01.sgetc();
  VERIFY( c1 != c2 );
  VERIFY( strmsz_2 != strmsz_1 );
  VERIFY( strmsz_2 == 1 );
  // end part three
  str_tmp = " ravi shankar meets carlos santana in LoHa   ";
  str_tmp += str_tmp;
  strmsz_1 = strb_01.str().size();
  strmsz_2 = strb_01.sputn(str_tmp.c_str(), str_tmp.size());
  strb_01.pubseekoff(0, std::ios_base::end);
  strb_01.sputc('<');
  str_tmp = strb_01.str();
  VERIFY(static_cast<std::streamsize>(str_tmp.size()) == strmsz_1 + strmsz_2 + 1);
  // IN
  // OUT
}

int main()
{
  test04();
  return 0;
}



// more candy!!!
