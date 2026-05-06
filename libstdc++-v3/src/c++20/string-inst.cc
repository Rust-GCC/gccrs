// string instantiations for C++20 -*- C++ -*-

// Copyright (C) 2026 Free Software Foundation, Inc.
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
// ISO C++ 14882:2020 21  Strings library
//

#ifndef _GLIBCXX_USE_CXX11_ABI
// Instantiations in this file use the new SSO std::string ABI unless included
// by another file which defines _GLIBCXX_USE_CXX11_ABI=0.
# define _GLIBCXX_USE_CXX11_ABI 1
#endif

#include <string>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
template basic_string<char>::basic_string() noexcept;
template bool basic_string<char>::starts_with(string_view) const noexcept;
template bool basic_string<char>::starts_with(char) const noexcept;
template bool basic_string<char>::starts_with(const char*) const noexcept;
template bool basic_string<char>::ends_with(string_view) const noexcept;
template bool basic_string<char>::ends_with(char) const noexcept;
template bool basic_string<char>::ends_with(const char*) const noexcept;

#ifdef _GLIBCXX_USE_WCHAR_T
template basic_string<wchar_t>::basic_string() noexcept;
template bool basic_string<wchar_t>::starts_with(wstring_view) const noexcept;
template bool basic_string<wchar_t>::starts_with(wchar_t) const noexcept;
template bool basic_string<wchar_t>::starts_with(const wchar_t*) const noexcept;
template bool basic_string<wchar_t>::ends_with(wstring_view) const noexcept;
template bool basic_string<wchar_t>::ends_with(wchar_t) const noexcept;
template bool basic_string<wchar_t>::ends_with(const wchar_t*) const noexcept;
#endif

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std
