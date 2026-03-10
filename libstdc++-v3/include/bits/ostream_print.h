// Inline implementation details for std::print functions -*- C++ -*-

// Copyright The GNU Toolchain Authors.
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

/** @file include/bits/ostream_print.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ostream}
 *
 *  This file contains the parts of `<ostream>` which are currently defined
 *  inline, but should be moved into the library eventually.
 */

#ifndef _GLIBCXX_OSTREAM_PRINT_H
#define _GLIBCXX_OSTREAM_PRINT_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <bits/requires_hosted.h> // for std::format

#include <bits/version.h>

#ifdef __glibcxx_print // C++ >= 23
#include <format>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#ifdef _GLIBCXX_NO_INLINE_PRINT
# define _GLIBCXX_PRINT_INLINE_USED [[__gnu__::__used__]]
#else
# define _GLIBCXX_PRINT_INLINE_USED
#endif

  _GLIBCXX_PRINT_INLINE_USED
  inline void
  vprint_nonunicode(ostream& __os, string_view __fmt, format_args __args)
  {
    ostream::sentry __cerb(__os);
    if (__cerb)
      {
	__format::_Str_sink<char> __buf;
	std::vformat_to(__buf.out(), __os.getloc(), __fmt, __args);
	auto __out = __buf.view();

	__try
	  {
	    std::__ostream_write(__os, __out.data(), __out.size());
	  }
	__catch(const __cxxabiv1::__forced_unwind&)
	  {
	    __os._M_setstate(ios_base::badbit);
	    __throw_exception_again;
	  }
	__catch(...)
	  { __os._M_setstate(ios_base::badbit); }
      }
  }

  _GLIBCXX_PRINT_INLINE_USED
  inline void
  vprint_unicode(ostream& __os, string_view __fmt, format_args __args)
  {
#if !defined(_WIN32) || defined(__CYGWIN__)
    // For most targets we don't need to do anything special to write
    // Unicode to a terminal.
    std::vprint_nonunicode(__os, __fmt, __args);
#else
    ostream::sentry __cerb(__os);
    if (__cerb)
      {
	__format::_Str_sink<char> __buf;
	std::vformat_to(__buf.out(), __os.getloc(), __fmt, __args);
	auto __out = __buf._M_span();

	void* __open_terminal(streambuf*);
	error_code __write_to_terminal(void*, span<char>);
	// If stream refers to a terminal, write a Unicode string to it.
	if (auto __term = __open_terminal(__os.rdbuf()))
	  {
#if !defined(_WIN32) || defined(__CYGWIN__)
	    // For POSIX, __open_terminal(streambuf*) uses fdopen to open a
	    // new file, so we would need to close it here. This code is not
	    // actually compiled because it's inside an #ifdef _WIN32 group,
	    // but just in case that changes in future ...
	    struct _Guard
	    {
	      _Guard(void* __p) : _M_f((FILE*)__p) { }
	      ~_Guard() { std::fclose(_M_f); }
	      _Guard(_Guard&&) = delete;
	      _Guard& operator=(_Guard&&) = delete;
	      FILE* _M_f;
	    };
	    _Guard __g(__term);
#endif

	    ios_base::iostate __err = ios_base::goodbit;
	    __try
	      {
		if (__os.rdbuf()->pubsync() == -1)
		  __err = ios::badbit;
		else if (auto __e = __write_to_terminal(__term, __out))
		  if (__e != std::make_error_code(errc::illegal_byte_sequence))
		    __err = ios::badbit;
	      }
	    __catch(const __cxxabiv1::__forced_unwind&)
	      {
		__os._M_setstate(ios_base::badbit);
		__throw_exception_again;
	      }
	    __catch(...)
	      { __os._M_setstate(ios_base::badbit); }

	    if (__err)
	      __os.setstate(__err);
	    return;
	  }

	// Otherwise just insert the string as vprint_nonunicode does.
	__try
	  {
	    std::__ostream_write(__os, __out.data(), __out.size());
	  }
	__catch(const __cxxabiv1::__forced_unwind&)
	  {
	    __os._M_setstate(ios_base::badbit);
	    __throw_exception_again;
	  }
	__catch(...)
	  { __os._M_setstate(ios_base::badbit); }
      }
#endif // _WIN32
  }
#undef _GLIBCXX_PRINT_INLINE_USED

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std
#endif // __glibcxx_print
#endif // _GLIBCXX_OSTREAM_PRINT_H
