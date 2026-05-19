// <iosfwd> Forward declarations -*- C++ -*-

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

/** @file include/iosfwd
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */

#ifndef _GLIBCXX_IOSFWD_H
#define _GLIBCXX_IOSFWD_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <bits/requires_hosted.h> // iostreams

#include <bits/c++config.h>
#include <bits/char_traits.h> 	// For char_traits, streamoff, streamsize, fpos

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup io
   *  @{
  */
  class ios_base;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ios;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_streambuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_istream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ostream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_iostream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class istreambuf_iterator;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class ostreambuf_iterator;


  /// Base class for @c char streams.
  typedef basic_ios<char> 		ios;

  /// Base class for @c char buffers.
  typedef basic_streambuf<char> 	streambuf;

  /// Base class for @c char input streams.
  typedef basic_istream<char> 		istream;

  /// Base class for @c char output streams.
  typedef basic_ostream<char> 		ostream;

  /// Base class for @c char mixed input and output streams.
  typedef basic_iostream<char> 		iostream;


#ifdef _GLIBCXX_USE_WCHAR_T
  /// Base class for @c wchar_t streams.
  typedef basic_ios<wchar_t> 		wios;

  /// Base class for @c wchar_t buffers.
  typedef basic_streambuf<wchar_t> 	wstreambuf;

  /// Base class for @c wchar_t input streams.
  typedef basic_istream<wchar_t> 	wistream;

  /// Base class for @c wchar_t output streams.
  typedef basic_ostream<wchar_t> 	wostream;

  /// Base class for @c wchar_t mixed input and output streams.
  typedef basic_iostream<wchar_t> 	wiostream;
#endif

  /** @}  */

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif /* _GLIBCXX_IOSFWD_H */
