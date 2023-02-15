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

// { dg-do compile { target c++11 } }

#include <chrono>

namespace chrono = std::chrono;

// A duration-like type
struct durayshun : chrono::seconds
{
  using duration::duration;
  durayshun(chrono::seconds);
};

chrono::time_point<chrono::system_clock, durayshun> t; // { dg-error "here" }
// { dg-error "specialization of std::chrono::duration" "" { target *-*-* } 0 }
