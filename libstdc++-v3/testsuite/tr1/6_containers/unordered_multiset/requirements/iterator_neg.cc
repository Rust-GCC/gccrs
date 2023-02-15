// 2005-10-02  Paolo Carlini  <pcarlini@suse.de>
//
// Copyright (C) 2005-2023 Free Software Foundation, Inc.
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
//

// { dg-do compile }

#include <tr1/unordered_set>

void test01()
{
  typedef std::tr1::unordered_multiset<int> Mset;

  Mset ms;

  Mset::const_iterator cit = ms.begin();
  *cit = 0; // { dg-error "read-only" }

  Mset::const_local_iterator clit = ms.begin(0);
  *clit = 0; // { dg-error "read-only" }
}
