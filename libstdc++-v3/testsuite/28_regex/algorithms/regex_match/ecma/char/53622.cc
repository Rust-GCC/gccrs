// { dg-do run { target c++11 } }
// { dg-timeout-factor 2 }

//
// 2013-07-23  Tim Shen <timshen91@gmail.com>
//
// Copyright (C) 2013-2023 Free Software Foundation, Inc.
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

#include <regex>
#include <testsuite_hooks.h>
#include <testsuite_regex.h>

using namespace __gnu_test;
using namespace std;

// libstdc++/53622
void
test01()
{
  {
    std::regex  re("zxcv/(one.*)abc", std::regex::ECMAScript);
    std::string target("zxcv/onetwoabc");
    std::smatch m;

    VERIFY( regex_match_debug(target, m, re) );
    VERIFY( m.size() == 2 );
    VERIFY( m[0].matched == true );
    VERIFY( std::string(m[0].first, m[0].second) == "zxcv/onetwoabc" );
    VERIFY( m[1].matched == true );
    VERIFY( std::string(m[1].first, m[1].second) == "onetwo" );
  }

  {
    std::regex  re("zxcv/(one.*)abc()\\2", std::regex::ECMAScript);
    std::string target("zxcv/onetwoabc");
    std::smatch m;

    VERIFY( regex_match_debug(target, m, re) );
    VERIFY( m.size() == 3 );
    VERIFY( m[0].matched == true );
    VERIFY( std::string(m[0].first, m[0].second) == "zxcv/onetwoabc" );
    VERIFY( m[1].matched == true );
    VERIFY( std::string(m[1].first, m[1].second) == "onetwo" );
  }
}

int
main()
{
  test01();
  return 0;
}
