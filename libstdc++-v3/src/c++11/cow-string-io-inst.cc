// Reference-counted COW string instantiations for I/O -*- C++ -*-

// Copyright (C) 2014-2023 Free Software Foundation, Inc.
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

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

//
// ISO C++ 14882: 21  Strings library
//

#define _GLIBCXX_USE_CXX11_ABI 0
#include <istream>
#include <ostream>

#if ! _GLIBCXX_USE_DUAL_ABI
# error This file should not be compiled for this configuration.
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // The equivalent SSO string instantiations are in c++98/misc-inst.cc,
  // repeat them for COW string

  // string related to iostreams.
  template
    basic_istream<char>&
    operator>>(basic_istream<char>&, string&);
  template
    basic_ostream<char>&
    operator<<(basic_ostream<char>&, const string&);
  template
    basic_istream<char>&
    getline(basic_istream<char>&, string&, char);
  template
    basic_istream<char>&
    getline(basic_istream<char>&, string&);

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
