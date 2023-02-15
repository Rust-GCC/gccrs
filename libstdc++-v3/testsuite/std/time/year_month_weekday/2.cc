// { dg-options "-std=gnu++2a" }
// { dg-do compile { target c++2a } }

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

// Class template year_month_weekday [time.cal.year_month_weekday]

#include <chrono>

constexpr void
constexpr_year_month_weekday_op_overload_disambiguation()
{
  using namespace std::chrono;
  using decades = duration<long long, std::ratio<31556952 * 10>>;
  static_assert(std::convertible_to<decades, months>
		&& std::convertible_to<decades, years>);
  using ymwd = year_month_weekday;

  constexpr ymwd ymwd1 = 2015y/June/Monday[3];
  static_assert(ymwd1 + decades{1} == 2025y/June/Monday[3]);
  static_assert(ymwd1 - decades{1} == 2005y/June/Monday[3]);
  static_assert(decades{1} + ymwd1 == 2025y/June/Monday[3]);
  static_assert((ymwd{ymwd1} += decades{1}) == 2025y/June/Monday[3]);
  static_assert((ymwd{ymwd1} -= decades{1}) == 2005y/June/Monday[3]);
}
