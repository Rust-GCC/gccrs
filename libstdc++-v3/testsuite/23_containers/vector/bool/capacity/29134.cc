// Copyright (C) 2006-2019 Free Software Foundation, Inc.
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

// 23.2.5 class vector<bool> [lib.vector.bool]

#include <vector>
#include <limits>
#include <testsuite_hooks.h>

// libstdc++/29134
void test01()
{
  using std::vector;
  using std::numeric_limits;

#ifdef _GLIBCXX_DEBUG
  using std::_GLIBCXX_STD_C::_S_word_bit;
#else
  using std::_S_word_bit;
#endif

  // Actually, vector<bool> is special, see libstdc++/31370.
  vector<bool> vb;
  typedef vector<bool>::difference_type difference_type;
  typedef vector<bool>::size_type size_type;
  VERIFY( vb.max_size()
	  == size_type(numeric_limits<difference_type>::max()
		       - int(_S_word_bit) + 1) );
}

int main()
{
  test01();
  return 0;
}
