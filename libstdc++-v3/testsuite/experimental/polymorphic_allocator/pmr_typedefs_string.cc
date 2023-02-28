// Copyright (C) 2018-2023 Free Software Foundation, Inc.
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

// { dg-do compile { target c++14 } }
// { dg-require-effective-target cxx11_abi }

#include <experimental/string>

namespace xpmr = std::experimental::pmr;

struct T : std::char_traits<char> { };

static_assert(std::is_same<xpmr::basic_string<char>,
    std::basic_string<char, std::char_traits<char>,
		      xpmr::polymorphic_allocator<char>>>::value,
    "pmr::basic_string");
static_assert(std::is_same<xpmr::basic_string<char, T>,
    std::basic_string<char, T, xpmr::polymorphic_allocator<char>>>::value,
    "pmr::basic_string");

static_assert(std::is_same<xpmr::string,
    std::basic_string<char, std::char_traits<char>,
		      xpmr::polymorphic_allocator<char>>>::value,
    "pmr::string");
#ifdef _GLIBCXX_USE_CHAR8_T
static_assert(std::is_same<xpmr::u8string,
    std::basic_string<char8_t, std::char_traits<char8_t>,
		      xpmr::polymorphic_allocator<char8_t>>>::value,
    "pmr::u8string");
#endif
static_assert(std::is_same<xpmr::u16string,
    std::basic_string<char16_t, std::char_traits<char16_t>,
		      xpmr::polymorphic_allocator<char16_t>>>::value,
    "pmr::u16string");
static_assert(std::is_same<xpmr::u32string,
    std::basic_string<char32_t, std::char_traits<char32_t>,
		      xpmr::polymorphic_allocator<char32_t>>>::value,
    "pmr::u32string");

static_assert(std::is_same<xpmr::basic_string<wchar_t>,
    std::basic_string<wchar_t, std::char_traits<wchar_t>,
		      xpmr::polymorphic_allocator<wchar_t>>>::value,
    "pmr::basic_string<wchar_t>");
static_assert(std::is_same<xpmr::basic_string<wchar_t, T>,
    std::basic_string<wchar_t, T, xpmr::polymorphic_allocator<wchar_t>>>::value,
    "pmr::basic_string<wchar_t>");

static_assert(std::is_same<xpmr::wstring,
    std::basic_string<wchar_t, std::char_traits<wchar_t>,
		      xpmr::polymorphic_allocator<wchar_t>>>::value,
    "pmr::wstring");
