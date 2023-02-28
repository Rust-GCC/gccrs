// { dg-do run { target c++11 } }
// { dg-timeout-factor 2 }

//
// 2010-06-16  Stephen M. Webb <stephen.webb@bregmasoft.ca>
//
// Copyright (C) 2010-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// 28.11.2 regex_match
// Tests ERE against a std::string target, exercising range {2,3}

#include <regex>
#include <testsuite_hooks.h>
#include <testsuite_regex.h>

using namespace __gnu_test;
using namespace std;

void
test01()
{
  std::regex  re("a{2,3}", std::regex::extended);
  std::string target("aa");
  std::smatch m;

  VERIFY( regex_match_debug(target, m, re) );

  VERIFY( m.size()  == re.mark_count()+1 );
  VERIFY( m.empty() == false );
  VERIFY( m.prefix().first == target.begin() );
  VERIFY( m.prefix().second == target.begin() );
  VERIFY( m.prefix().matched == false );
  VERIFY( m.suffix().first == target.end() );
  VERIFY( m.suffix().second == target.end() );
  VERIFY( m.suffix().matched == false );
  VERIFY( m[0].first == target.begin() );
  VERIFY( m[0].second == target.end() );
  VERIFY( m[0].matched == true );
}


int
main()
{ 
  test01();
  return 0;
}

