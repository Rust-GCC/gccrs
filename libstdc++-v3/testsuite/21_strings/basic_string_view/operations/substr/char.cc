// { dg-do run { target c++17 } }

// Copyright (C) 2013-2023 Free Software Foundation, Inc.
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

// basic_string_view::substr

#include <string_view>
#include <testsuite_hooks.h>

#if __STDC_HOSTED__
# include <stdexcept>
#endif

void
test01()
{
  typedef std::string_view::size_type csize_type;
  typedef std::string_view::const_reference cref;
  typedef std::string_view::reference ref;
  csize_type csz01;

  const char str_lit01[] = "rockaway, pacifica";
  const std::string_view str01(str_lit01);
  std::string_view str02;

  // basic_string_view<charT, _Traits, _Alloc>
  //  substr(size_type pos = 0, size_type n = npos) const;
  csz01 = str01.size();
  str02 = str01.substr(0, 1);
  VERIFY( str02 == "r" );
  str02 = str01.substr(10);
  VERIFY( str02 == "pacifica" );

#if __STDC_HOSTED__
  try
  {
    str02 = str01.substr(csz01 + 1);
    VERIFY( false ); 
  }
  catch(std::out_of_range& fail)
  {
    VERIFY( true );
  }
  catch(...)
  {
    VERIFY( false );
  }

  try
  {
    str02 = str01.substr(csz01);
    VERIFY( str02.size() == 0 );
    VERIFY( str02.begin() == str01.end() );
    VERIFY( true );
  }
  catch(...)
  {
    VERIFY( false );
  }
#endif // HOSTED
}

int
main()
{ 
  test01();

  return 0;
}
