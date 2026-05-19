// <iosfwd> Forward declarations for <sstream> -*- C++ -*-

// Copyright (C) 1997-2026 Free Software Foundation, Inc.
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

/** @file include/bits/iosfwd_string.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */

#ifndef _GLIBCXX_IOSFWD_STRING_H
#define _GLIBCXX_IOSFWD_STRING_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <bits/requires_hosted.h> // iostreams

#include <bits/iosfwd.h>
#include <bits/stringfwd.h> 	// For string forward declarations.

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup io
   *  @{
  */

_GLIBCXX_BEGIN_NAMESPACE_CXX11

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
	    typename _Alloc = allocator<_CharT> >
    class basic_stringbuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
	   typename _Alloc = allocator<_CharT> >
    class basic_istringstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
	   typename _Alloc = allocator<_CharT> >
    class basic_ostringstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
	   typename _Alloc = allocator<_CharT> >
    class basic_stringstream;

_GLIBCXX_END_NAMESPACE_CXX11

  /// Class for @c char memory buffers.
  typedef basic_stringbuf<char> 	stringbuf;

  /// Class for @c char input memory streams.
  typedef basic_istringstream<char> 	istringstream;

  /// Class for @c char output memory streams.
  typedef basic_ostringstream<char> 	ostringstream;

  /// Class for @c char mixed input and output memory streams.
  typedef basic_stringstream<char> 	stringstream;

#ifdef _GLIBCXX_USE_WCHAR_T
  /// Class for @c wchar_t memory buffers.
  typedef basic_stringbuf<wchar_t> 	wstringbuf;

  /// Class for @c wchar_t input memory streams.
  typedef basic_istringstream<wchar_t> 	wistringstream;

  /// Class for @c wchar_t output memory streams.
  typedef basic_ostringstream<wchar_t> 	wostringstream;

  /// Class for @c wchar_t mixed input and output memory streams.
  typedef basic_stringstream<wchar_t> 	wstringstream;
#endif
  /// @}

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif /* _GLIBCXX_IOSFWD_STRING_H */
