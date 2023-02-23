// { dg-do compile { target c++17 } }

// Copyright (C) 2016-2023 Free Software Foundation, Inc.
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

#include <memory>

// Not swappable, and unique_ptr not swappable via the generic std::swap.
struct C {
  void operator()(void*) const { }
};
void swap(C&, C&) = delete;

static_assert( !std::is_swappable_v<std::unique_ptr<int, C>> );

// Not swappable, and unique_ptr not swappable via the generic std::swap.
struct D {
  D(D&&) = delete;
  void operator()(void*) const { }
};

static_assert( !std::is_swappable_v<std::unique_ptr<int, D>> );

