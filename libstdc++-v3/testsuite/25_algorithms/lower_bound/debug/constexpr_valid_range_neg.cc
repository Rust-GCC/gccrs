// Copyright (C) 2020-2023 Free Software Foundation, Inc.
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

// { dg-options "-std=gnu++2a" }
// { dg-do compile { target c++2a } }
// { dg-require-debug-mode { } }

#include <algorithm>
#include <array>

constexpr bool
test1()
{
  constexpr std::array<int, 12> ca0{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};

  const auto outbb = std::lower_bound(ca0.end(), ca0.begin(), 6);

  return true;
}

static_assert(test1()); // { dg-error "" }

constexpr bool
test2()
{
  constexpr std::array<int, 12> ca0{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};

  const auto outcc = std::lower_bound(ca0.end(), ca0.begin(), 6,
				      std::less<int>());

  return true;
}

static_assert(test2()); // { dg-error "" }

// { dg-prune-output "builtin_unreachable" }
// { dg-prune-output "in 'constexpr'" }

