// 2000-09-11 Benjamin Kosnik <bkoz@redhat.com>

// Copyright (C) 2000-2023 Free Software Foundation, Inc.
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

// 22.1.1.4 locale operators [lib.locale.operators]

#include <cwchar> // for mbstate_t
#include <locale>
#include <testsuite_hooks.h>

// bool operator()(const string_type&, const string_type&) const
long gnu_count;

class gnu_collate: public std::collate<char>
{ 
protected:
  virtual int
  do_compare(const char*, const char*, const char*, const char*) const
  { ++gnu_count; return 0; }
}; 

void test02()
{
  using namespace std;
  
  // Sanity check.
  locale loc_c = locale::classic();
  string s01("land of ");
  string s02("land of look behind");
  VERIFY( !loc_c(s01, s01) );
  VERIFY( loc_c(s01, s02) );
 
  // Derivation, MF check.
  locale loc_gnu(loc_c, new gnu_collate);
  gnu_count = 0;
  loc_gnu(s01, s02);
  VERIFY( gnu_count == 1 );
}

int main ()
{
  test02();
  return 0;
}



