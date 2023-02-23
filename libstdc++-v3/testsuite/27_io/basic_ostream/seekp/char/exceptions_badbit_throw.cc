// Copyright (C) 2003-2023 Free Software Foundation, Inc.
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


#include <ostream>
#include <streambuf>
#include <sstream>
#include <testsuite_hooks.h>
#include <testsuite_io.h>

void test01()
{
  using namespace std;

  __gnu_test::fail_streambuf bib;
  ostream stream(&bib);

  ostream::pos_type pos = ostream::pos_type();

  try
    {
      stream.seekp(pos);
      VERIFY( false );
    }
  catch (const __gnu_test::positioning_error&)
    {
      VERIFY( stream.good() );
    }
  catch (...)
    {
      VERIFY( false );
    }
}

void test02()
{
  using namespace std;

  __gnu_test::fail_streambuf bib;
  ostream stream(&bib);

  ostream::off_type off(5);

  try
    {
      stream.seekp(off, ios_base::cur);
      VERIFY( false );
    }
  catch (const __gnu_test::positioning_error&)
    {
      VERIFY( stream.good() );
    }
  catch (...)
    {
      VERIFY( false );
    }
}

// libstdc++/9546
int main()
{
  test01();
  test02();
  return 0;
}
