// { dg-do run { target c++11 } }

// Copyright (C) 2008-2019 Free Software Foundation, Inc.
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

// 23.2.3.n forward_list capacity [lib.forward_list.capacity]

#include <forward_list>
#include <testsuite_hooks.h>

void
test01()
{
  std::forward_list<double> fld;
  VERIFY(fld.empty() == true);

  fld.push_front(1.0);
  VERIFY(fld.empty() == false);

  fld.resize(0);
  VERIFY(fld.empty() == true);

#ifdef _GLIBCXX_DEBUG
  using std::_GLIBCXX_STD_C::_Fwd_list_node;
#else
  using std::_Fwd_list_node;
#endif

  VERIFY( (fld.max_size()
	   == std::allocator<_Fwd_list_node<double> >().max_size()) );
}

int
main()
{
  test01();
  return 0;
}
