// <iosfwd> Forward declarations for <spanstream> -*- C++ -*-

// Copyright (C) 2022-2026 Free Software Foundation, Inc.
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

/** @file include/bits/iosfwd_span.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */

#ifndef _GLIBCXX_IOSFWD_SPAN_H
#define _GLIBCXX_IOSFWD_SPAN_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <bits/requires_hosted.h> // iostreams

#include <bits/version.h>

#ifdef __glibcxx_spanstream // >= C++23
#include <bits/iosfwd.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup io
   *  @{
  */

  template<typename _CharT, typename _Traits = char_traits<_CharT>>
    class basic_spanbuf;
  template<typename _CharT, typename _Traits = char_traits<_CharT>>
    class basic_ispanstream;
  template<typename _CharT, typename _Traits = char_traits<_CharT>>
    class basic_ospanstream;
  template<typename _CharT, typename _Traits = char_traits<_CharT>>
    class basic_spanstream;

  using spanbuf     = basic_spanbuf<char>;
  using ispanstream = basic_ispanstream<char>;
  using ospanstream = basic_ospanstream<char>;
  using spanstream  = basic_spanstream<char>;

#ifdef _GLIBCXX_USE_WCHAR_T
  using wspanbuf     = basic_spanbuf<wchar_t>;
  using wispanstream = basic_ispanstream<wchar_t>;
  using wospanstream = basic_ospanstream<wchar_t>;
  using wspanstream  = basic_spanstream<wchar_t>;
#endif
  /// @}

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
#endif // C++23
#endif /* _GLIBCXX_IOSFWD_SPAN_H */
