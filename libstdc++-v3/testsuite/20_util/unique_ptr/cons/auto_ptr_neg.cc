// { dg-options "-Wno-deprecated" }
// { dg-add-options using-deprecated }
// { dg-do compile { target c++11 } }

// Copyright (C) 2010-2023 Free Software Foundation, Inc.
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

// 20.9.10 Class template unique_ptr [unique.ptr]

#include <memory>

struct A { };

// 20.9.10.2.1 unique_ptr constructors [unique.ptr.single.ctor]

// Construction from const auto_ptr
void
test01()
{
  const std::auto_ptr<A> a(new A);
  std::unique_ptr<A> a2(std::move(a)); // { dg-error "no match" }
}
