// std::collate implementation details, DragonFly version -*- C++ -*-

// Copyright (C) 2015-2023 Free Software Foundation, Inc.
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
// ISO C++ 14882: 22.2.4.1.2  collate virtual functions
//

// Written by Benjamin Kosnik <bkoz@redhat.com>
// Modified for DragonFly by John Marino <gnugcc@marino.st>

#include <locale>
#include <cstring>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // These are basically extensions to char_traits, and perhaps should
  // be put there instead of here.
  template<>
    int
    collate<char>::_M_compare(const char* __one,
			      const char* __two) const throw()
    {
      int __cmp = strcoll_l(__one, __two, (locale_t)_M_c_locale_collate);
      return (__cmp >> (8 * sizeof (int) - 2)) | (__cmp != 0);
    }

  template<>
    size_t
    collate<char>::_M_transform(char* __to, const char* __from,
				size_t __n) const throw()
    { return strxfrm_l(__to, __from, __n, (locale_t)_M_c_locale_collate); }

#ifdef _GLIBCXX_USE_WCHAR_T
  template<>
    int
    collate<wchar_t>::_M_compare(const wchar_t* __one,
				 const wchar_t* __two) const throw()
    {
      int __cmp = wcscoll_l(__one, __two, (locale_t)_M_c_locale_collate);
      return (__cmp >> (8 * sizeof (int) - 2)) | (__cmp != 0);
    }

  template<>
    size_t
    collate<wchar_t>::_M_transform(wchar_t* __to, const wchar_t* __from,
				   size_t __n) const throw()
    { return wcsxfrm_l(__to, __from, __n, (locale_t)_M_c_locale_collate); }
#endif

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
