// Copyright (C) 2019-2023 Free Software Foundation, Inc.
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

#include <concepts>

static_assert( ! std::strict_weak_order<bool, void, void> );
static_assert( ! std::strict_weak_order<bool(), void, void> );
static_assert( ! std::strict_weak_order<bool(*)(), void, void> );
static_assert( ! std::strict_weak_order<bool(&)(int, int), void, void> );
static_assert( ! std::strict_weak_order<bool(), int, int> );
static_assert( ! std::strict_weak_order<bool() const, int, int> );

static_assert( std::strict_weak_order<bool(*)(int, int), short, long> );
static_assert( std::strict_weak_order<bool(&)(const void*, const void*), char[2], int*> );

static_assert( ! std::strict_weak_order<bool& (const long*, short), long*, char> );

struct A;
static_assert( ! std::strict_weak_order<int A::*, const A&, const A&> );
static_assert( ! std::strict_weak_order<void (A::*)(long&), const A*, long&> );
static_assert( ! std::strict_weak_order<void (A::*)(long&) const, A*, long&> );
static_assert( std::strict_weak_order<long (A::*)(A*) const, A*, A*> );

struct F
{
  void operator()(long, long) const;
  bool& operator()(int, const int&) const;
};
static_assert( ! std::strict_weak_order<F, long, long> );
static_assert( std::strict_weak_order<F&, int, int> );
static_assert( std::strict_weak_order<const F&, const int, const int> );
