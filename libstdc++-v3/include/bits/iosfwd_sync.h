// <iosfwd> Forward declarations for <syncstream> -*- C++ -*-

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

/** @file include/bits/iosfwd_sync.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */

#ifndef _GLIBCXX_IOSFWD_SYNC_H
#define _GLIBCXX_IOSFWD_SYNC_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <bits/requires_hosted.h> // iostreams

#include <bits/version.h>

#ifdef __glibcxx_syncbuf // >= C++20 && CXX11_ABI
#include <bits/iosfwd.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup io
   *  @{
  */

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
	   typename _Allocator = allocator<_CharT>>
    class basic_syncbuf;
  template<typename _CharT, typename _Traits = char_traits<_CharT>,
	   typename _Allocator = allocator<_CharT>>
    class basic_osyncstream;

  using syncbuf = basic_syncbuf<char>;
  using osyncstream = basic_osyncstream<char>;

#ifdef _GLIBCXX_USE_WCHAR_T
  using wsyncbuf = basic_syncbuf<wchar_t>;
  using wosyncstream = basic_osyncstream<wchar_t>;
#endif
  /// @}

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
#endif // __glibcxx_syncbuf
#endif /* _GLIBCXX_IOSFWD_SYNC_H */
