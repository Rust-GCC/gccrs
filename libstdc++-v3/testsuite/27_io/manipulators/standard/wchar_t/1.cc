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

// 27.6.3 - Standard manipulators

#include <sstream>
#include <iomanip>
#include <testsuite_hooks.h>

void
test01()
{
  using namespace std;

  wstring s(L"john coltrane, a love supreme");
  wistringstream  iss(s);
  wostringstream  oss;

  // resetiosflags
  resetiosflags(ios_base::boolalpha);
  iss >> resetiosflags(ios_base::boolalpha);
  VERIFY(iss.good());
  oss << resetiosflags(ios_base::boolalpha);
  VERIFY(oss.good());

  // setiosflags
  setiosflags(ios_base::skipws);
  iss >> setiosflags(ios_base::skipws);
  VERIFY(iss.good());
  oss << setiosflags(ios_base::skipws);
  VERIFY(oss.good());

  // setbase
  setbase(8);
  iss >> setbase(8);
  VERIFY(iss.good());
  oss << setbase(8);
  VERIFY(oss.good());

  // setfil
  setfill(L'a');
  iss >> setfill(L'a');
  VERIFY(iss.good());
  oss << setfill(L'a');
  VERIFY(oss.good());
 
  // setprecision
  setprecision(4);
  iss >> setprecision(4);
  VERIFY(iss.good());
  oss << setprecision(4);
  VERIFY(oss.good());
  
  // setprecision
  setw(7);
  iss >> setw(7);
  VERIFY(iss.good());
  oss << setw(7);
  VERIFY(oss.good());
}

int
main()
{
  test01();
  return 0;
}
