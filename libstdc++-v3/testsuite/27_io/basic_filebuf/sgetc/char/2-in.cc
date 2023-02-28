// 2001-05-21 Benjamin Kosnik  <bkoz@redhat.com>

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

// 27.8.1.4 Overridden virtual functions

// { dg-require-fileio "" }

#include <fstream>
#include <testsuite_hooks.h>
#include <testsuite_io.h>

// @require@ %-*.tst %-*.txt
// @diff@ %-*.tst %*.txt

const char name_01[] = "sgetc.txt"; // file with data in it

// Test overloaded virtual functions.
void test05() 
{
  using namespace std;
  using namespace __gnu_test;
  typedef std::filebuf::int_type 	int_type;
  typedef filebuf::traits_type 		traits_type;

  int_type 			c1, c2, c3;

  // int_type sgetc()
  // if read_cur not avail, return uflow(), else return *read_cur  

  // in
  {
    constraint_filebuf fb_01; // in 
    fb_01.pubsetbuf(0, 0);
    fb_01.open(name_01, ios::in);
    VERIFY( fb_01.unbuffered() );
    c1 = fb_01.sgetc();
    VERIFY( c1 == '/' );
    c2 = fb_01.sgetc();
    VERIFY( c1 == c2 );
    fb_01.sbumpc();
    c1 = fb_01.sbumpc();
    c2 = fb_01.sgetc();
    c3 = fb_01.sgetc();
    VERIFY( c1 == '/' );
    VERIFY( c2 == ' ' );
    VERIFY( c3 == ' ' );
    VERIFY( fb_01.unbuffered() );
  }
}

int main() 
{
  test05();
  return 0;
}
