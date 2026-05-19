// <iosfwd> Forward declarations for <fstream> -*- C++ -*-

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

/** @file include/bits/iosfwd_file.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */

#ifndef _GLIBCXX_IOSFWD_FILE_H
#define _GLIBCXX_IOSFWD_FILE_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <bits/requires_hosted.h> // iostreams

#include <bits/iosfwd.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup io
   *  @{
  */
  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_filebuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ifstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ofstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_fstream;

  /// Class for @c char file buffers.
  typedef basic_filebuf<char> 		filebuf;

  /// Class for @c char input file streams.
  typedef basic_ifstream<char> 		ifstream;

  /// Class for @c char output file streams.
  typedef basic_ofstream<char> 		ofstream;

  /// Class for @c char mixed input and output file streams.
  typedef basic_fstream<char> 		fstream;

#ifdef _GLIBCXX_USE_WCHAR_T
    /// Class for @c wchar_t file buffers.
  typedef basic_filebuf<wchar_t> 	wfilebuf;

  /// Class for @c wchar_t input file streams.
  typedef basic_ifstream<wchar_t> 	wifstream;

  /// Class for @c wchar_t output file streams.
  typedef basic_ofstream<wchar_t> 	wofstream;

  /// Class for @c wchar_t mixed input and output file streams.
  typedef basic_fstream<wchar_t> 	wfstream;
#endif
  /// @}

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif /* _GLIBCXX_IOSFWD_FILE_H */
