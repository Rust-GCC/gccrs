// { dg-do compile { target c++17 } }

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

// { dg-error "must be a complete class" "" { target *-*-* } 0 }

#include <type_traits>

class X;

void test01()
{
  std::is_nothrow_invocable<int(X), X>();		// { dg-error "required from here" }
  std::is_nothrow_invocable<int(int, X), int, X>();		// { dg-error "required from here" }
  std::is_nothrow_invocable<int(int, X), X, int>();		// { dg-error "required from here" }


  std::is_nothrow_invocable<int(X&), X&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable<int(int, X&), int, X&>();		// { dg-bogus "required from here" }

  std::is_nothrow_invocable<int(X&&), X&&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable<int(int, X&&), int, X&&>();		// { dg-bogus "required from here" }

  std::is_nothrow_invocable<int(const X&&), const X&&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable<int(int, const X&&), int, const X&&>();		// { dg-bogus "required from here" }

  std::is_nothrow_invocable<int(const X&), const X&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable<int(int, const X&), int, const X&>();		// { dg-bogus "required from here" }

  std::is_nothrow_invocable<int(const X&), X&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable<int(int, const X&), int, X&>();		// { dg-bogus "required from here" }
}

void test02()
{
  std::is_nothrow_invocable_r<int, int(X), X>();		// { dg-error "required from here" }
  std::is_nothrow_invocable_r<int, int(int, X), int, X>();		// { dg-error "required from here" }
  std::is_nothrow_invocable_r<int, int(int, X), X, int>();		// { dg-error "required from here" }


  std::is_nothrow_invocable_r<int, int(X&), X&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable_r<int, int(int, X&), int, X&>();		// { dg-bogus "required from here" }

  std::is_nothrow_invocable_r<int, int(X&&), X&&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable_r<int, int(int, X&&), int, X&&>();		// { dg-bogus "required from here" }

  std::is_nothrow_invocable_r<int, int(const X&&), const X&&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable_r<int, int(int, const X&&), int, const X&&>();		// { dg-bogus "required from here" }

  std::is_nothrow_invocable_r<int, int(const X&), const X&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable_r<int, int(int, const X&), int, const X&>();		// { dg-bogus "required from here" }

  std::is_nothrow_invocable_r<int, int(const X&), X&>();		// { dg-bogus "required from here" }
  std::is_nothrow_invocable_r<int, int(int, const X&), int, X&>();		// { dg-bogus "required from here" }
}
