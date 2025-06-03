// <bits/erase_if.h> -*- C++ -*-

// Copyright (C) 2015-2025 Free Software Foundation, Inc.
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

/** @file bits/erase_if.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly.
 */

#ifndef _GLIBCXX_ERASE_IF_H
#define _GLIBCXX_ERASE_IF_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <bits/c++config.h>

// Used by C++17 containers and Library Fundamentals v2 headers.
#if __cplusplus >= 201402L
namespace std
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace __detail
  {
    template<typename _Container, typename _UnsafeContainer,
	     typename _Predicate>
      typename _Container::size_type
      __erase_nodes_if(_Container& __cont, _UnsafeContainer& __ucont,
		       _Predicate __pred)
      {
	typename _Container::size_type __num = 0;
	for (auto __iter = __ucont.begin(), __last = __ucont.end();
	     __iter != __last;)
	  {
	    if (__pred(*__iter))
	      {
		__iter = __cont.erase(__iter);
		++__num;
	      }
	    else
	      ++__iter;
	  }
	return __num;
      }
  } // namespace __detail

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std
#endif // C++14

#endif // _GLIBCXX_ERASE_IF_H
