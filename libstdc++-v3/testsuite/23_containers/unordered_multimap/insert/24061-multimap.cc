// { dg-do run { target c++11 } }

// 2010-02-10  Paolo Carlini  <paolo.carlini@oracle.com> 
//
// Copyright (C) 2010-2019 Free Software Foundation, Inc.
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

#include <unordered_map>
#include <string>
#include <testsuite_hooks.h>

// libstdc++/24061
void test01()
{
  typedef std::unordered_multimap<std::string, int> Mmap;
  typedef Mmap::iterator       iterator;
  typedef Mmap::const_iterator const_iterator;
  typedef Mmap::value_type     value_type;

  Mmap mm1;
  mm1.reserve(3);

  iterator it1 = mm1.insert(mm1.begin(),
			    value_type("all the love in the world", 1));
  VERIFY( mm1.size() == 1 );
  VERIFY( *it1 == value_type("all the love in the world", 1) );
  
  const_iterator cit1(it1);
  const_iterator cit2 = mm1.insert(cit1,
				   value_type("you know what you are?", 2));
  VERIFY( mm1.size() == 2 );
  VERIFY( cit2 != cit1 );
  VERIFY( *cit2 == value_type("you know what you are?", 2) );

  iterator it2 = mm1.insert(it1, value_type("all the love in the world", 3));
  VERIFY( mm1.size() == 3 );
  VERIFY( it2 != it1 );
  VERIFY( *it2 == value_type("all the love in the world", 3) );
}
  
int main()
{
  test01();
  return 0;
}
