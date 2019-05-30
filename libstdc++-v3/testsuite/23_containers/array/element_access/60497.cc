// { dg-do compile { target c++11 } }
//
// Copyright (C) 2014-2019 Free Software Foundation, Inc.
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

// libstdc++/60497

#include <array>
#include <debug/array>
#include <memory>

struct A;
template<typename T> struct B { T t; };

std::array<B<A>*, 1> a;
auto b = std::get<0>(std::move(a));

std::__debug::array<B<A>*, 1> c;
auto d = std::__debug::get<0>(std::move(c));
