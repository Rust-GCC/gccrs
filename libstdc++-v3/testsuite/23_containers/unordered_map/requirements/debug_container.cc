// { dg-do compile { target c++11 } }

// Copyright (C) 2007-2019 Free Software Foundation, Inc.
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

#include <string>
#include <debug/unordered_map>

using std::allocator;
using std::equal_to;
using std::hash;
using std::pair;
using std::string;

template class __gnu_debug::unordered_map<string, float>;
template class __gnu_debug::unordered_map<string, int,
                                          hash<string>, equal_to<string>, 
                                          allocator<pair<const string, int>>>;
#if !defined __STRICT_ANSI__ && __cplusplus <= 201703L
template class __gnu_debug::unordered_map<string, float,
                                          hash<string>, equal_to<string>, 
                                          allocator<char>>;
#endif
