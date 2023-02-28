// -*- C++ -*-

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

// { dg-do compile { target c++11 } }

#include <memory>
#include <iterator>
#include <debug/vector>

class Foo
{};

void
test01()
{
  __gnu_debug::vector<std::unique_ptr<Foo>> v;
  __gnu_debug::vector<std::unique_ptr<Foo>> w;

  v.insert(end(v),
	   make_move_iterator(begin(w)),
	   make_move_iterator(end(w)));
}
