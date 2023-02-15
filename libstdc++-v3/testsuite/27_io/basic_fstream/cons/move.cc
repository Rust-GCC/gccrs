// Copyright (C) 2014-2023 Free Software Foundation, Inc.
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

// { dg-do run { target c++11 } }
// { dg-require-fileio "" }

// 27.9.1.15 basic_fstream constructors [fstream.cons]

#include <fstream>
#include <string>
#include <testsuite_hooks.h>

using namespace std;

std::string const name = "fstream-move.txt";

void
test01()
{
  string orig = "Let the whole outside world consist of a long paper tape.";
  fstream f(name, ios::in|ios::out|ios::trunc);
  VERIFY( f.is_open() );
  f << orig;
  fstream f1 = std::move(f);
  VERIFY( f1.is_open() );
  VERIFY( !f.is_open() );
  f1.seekg(0);
  string result;
  getline(f1, result);
  VERIFY( result == orig );
}

void
test02()
{
#ifdef _GLIBCXX_USE_WCHAR_T
  wstring orig = L"Let the whole outside world consist of a long paper tape.";
  wfstream f(name, ios::in|ios::out|ios::trunc);
  VERIFY( f.is_open() );
  f << orig;
  wfstream f1 = std::move(f);
  VERIFY( f1.is_open() );
  VERIFY( !f.is_open() );
  f1.seekg(0);
  wstring result;
  getline(f1, result);
  VERIFY( result == orig );
#endif
}

int
main()
{
  test01();
  test02();
}
