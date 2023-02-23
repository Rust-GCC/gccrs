// Copyright (C) 2005-2023 Free Software Foundation, Inc.
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
#include <testsuite_hooks.h>

// libstdc++/9561
struct foobar: std::exception { };

struct buf: std::wstreambuf
{
  virtual int_type
  overflow(int_type)
  {
    throw foobar();
    return int_type();
  }
};

void test01()
{
  using namespace std;

  buf b;
  std::wostream strm(&b);
  strm.exceptions(std::wios::badbit);

  try
    {
      strm << std::endl;
    }
  catch(const foobar&)
    {
      // strm should throw foobar and not do anything else
      VERIFY(strm.bad());
      return;
    }
  catch(...)
    {
      VERIFY( false );
      return;
    }
  VERIFY( false );
}

int main()
{
  test01();
  return 0;
}
