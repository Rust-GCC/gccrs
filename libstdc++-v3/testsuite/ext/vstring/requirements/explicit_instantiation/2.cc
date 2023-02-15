// Copyright (C) 2005-2023 Free Software Foundation, Inc.
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


// This file tests explicit instantiation of __versa_string

#include <ext/vstring.h>

// { dg-do compile }

// libstdc++/21770
template class __gnu_cxx::__versa_string<int, std::char_traits<int>,
					 std::allocator<char>,
					 __gnu_cxx::__sso_string_base>;

template class __gnu_cxx::__versa_string<int, std::char_traits<int>,
					 std::allocator<char>,
					 __gnu_cxx::__rc_string_base>;
