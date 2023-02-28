// { dg-do compile }

// 2007-02-04  Benjamin Kosnik  <bkoz@redhat.com>
//
// Copyright (C) 2007-2023 Free Software Foundation, Inc.
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
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <tr1/functional>

namespace std {
namespace tr1 {

  template <class T> class reference_wrapper;
  template <class T> reference_wrapper<T> ref(T&);
  template <class T> reference_wrapper<const T> cref(const T&);
  template <class T> reference_wrapper<T> ref(reference_wrapper<T>);
  template <class T> reference_wrapper<const T> cref(reference_wrapper<T>);

} // namespace tr1
} // namespace std
