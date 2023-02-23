// { dg-do compile }
// -*- C++ -*-

// Copyright (C) 2006-2023 Free Software Foundation, Inc.
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

#include <ext/type_traits.h>
#include <tr1/type_traits>

template<typename T>
  void
  check_remove_unsigned()
  {
    typedef typename __gnu_cxx::__remove_unsigned<T>::__type signed_type;
  }

int main()
{
  check_remove_unsigned<bool>();  // { dg-error "required from" }
  check_remove_unsigned<wchar_t>();  // { dg-error "required from" }
  return 0;
}

// { dg-error "invalid use of incomplete" "" { target *-*-* } 28 } 
// { dg-error "declaration of" "" { target *-*-* } 143 }
// { dg-error "declaration of" "" { target *-*-* } 146 }
