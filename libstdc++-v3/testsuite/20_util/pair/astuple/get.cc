// { dg-do compile { target c++11 } }

// 2011-05-16  Paolo Carlini  <paolo.carlini@oracle.com>
//
// Copyright (C) 2011-2023 Free Software Foundation, Inc.
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

#include <utility>

void test01()
{
  std::pair<float, int> p;

  float&& pfirst __attribute__((unused)) = std::get<0>(std::move(p));
  int&&  psecond __attribute__((unused)) = std::get<1>(std::move(p));

  const std::pair<float, int> cp;

  const float&& cpfirst __attribute__((unused)) = std::get<0>(std::move(cp));
  const int&&  cpsecond __attribute__((unused)) = std::get<1>(std::move(cp));
}
