// { dg-do compile { target c++11 } }

// Copyright (C) 2017-2023 Free Software Foundation, Inc.
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

// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.


// This file tests explicit instantiation of library containers.

#include <stack>

using std::stack;

template<typename A>
constexpr bool default_constructible()
{ return std::is_default_constructible<A>::value; }

static_assert(default_constructible<stack<int>>(), "stack<int>");

struct NonDefaultConstructible : std::deque<int> {
  NonDefaultConstructible(int) { }
};
static_assert(!default_constructible<stack<int, NonDefaultConstructible>>(),
	      "stack<int, NonDefaultConstructible>");
