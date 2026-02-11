// Implementation of <simd> -*- C++ -*-

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

#ifndef _GLIBCXX_SIMD_ITERATOR_H
#define _GLIBCXX_SIMD_ITERATOR_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#if __cplusplus >= 202400L

#include "simd_details.h"

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace simd
{
  /** @internal
   * Iterator type for basic_vec and basic_mask.
   *
   * C++26 [simd.iterator]
   */
  template <typename _Vp>
    class __iterator
    {
      friend class __iterator<const _Vp>;

      template <typename, typename>
	friend class _VecBase;

      template <size_t, typename>
	friend class _MaskBase;

      _Vp* _M_data = nullptr;

      __simd_size_type _M_offset = 0;

      constexpr
      __iterator(_Vp& __d, __simd_size_type __off)
      : _M_data(&__d), _M_offset(__off)
      {}

    public:
      using value_type = typename _Vp::value_type;

      using iterator_category = input_iterator_tag;

      using iterator_concept = random_access_iterator_tag;

      using difference_type = __simd_size_type;

      constexpr __iterator() = default;

      constexpr
      __iterator(const __iterator &) = default;

      constexpr __iterator&
      operator=(const __iterator &) = default;

      constexpr
      __iterator(const __iterator<remove_const_t<_Vp>> &__i) requires is_const_v<_Vp>
      : _M_data(__i._M_data), _M_offset(__i._M_offset)
      {}

      constexpr value_type
      operator*() const
      { return (*_M_data)[_M_offset]; } // checked in operator[]

      constexpr __iterator&
      operator++()
      {
	++_M_offset;
	return *this;
      }

      constexpr __iterator
      operator++(int)
      {
	__iterator r = *this;
	++_M_offset;
	return r;
      }

      constexpr __iterator&
      operator--()
      {
	--_M_offset;
	return *this;
      }

      constexpr __iterator
      operator--(int)
      {
	__iterator r = *this;
	--_M_offset;
	return r;
      }

      constexpr __iterator&
      operator+=(difference_type __x)
      {
	_M_offset += __x;
	return *this;
      }

      constexpr __iterator&
      operator-=(difference_type __x)
      {
	_M_offset -= __x;
	return *this;
      }

      constexpr value_type
      operator[](difference_type __i) const
      { return (*_M_data)[_M_offset + __i]; } // checked in operator[]

      constexpr friend bool operator==(__iterator __a, __iterator __b) = default;

      constexpr friend bool operator==(__iterator __a, std::default_sentinel_t) noexcept
      { return __a._M_offset == _Vp::size.value; }

      constexpr friend auto operator<=>(__iterator __a, __iterator __b)
      { return __a._M_offset <=> __b._M_offset; }

      constexpr friend __iterator
      operator+(const __iterator& __it, difference_type __x)
      { return __iterator(*__it._M_data, __it._M_offset + __x); }

      constexpr friend __iterator
      operator+(difference_type __x, const __iterator& __it)
      { return __iterator(*__it._M_data, __it._M_offset + __x); }

      constexpr friend __iterator
      operator-(const __iterator& __it, difference_type __x)
      { return __iterator(*__it._M_data, __it._M_offset - __x); }

      constexpr friend difference_type
      operator-(__iterator __a, __iterator __b)
      { return __a._M_offset - __b._M_offset; }

      constexpr friend difference_type
      operator-(__iterator __it, std::default_sentinel_t) noexcept
      { return __it._M_offset - difference_type(_Vp::size.value); }

      constexpr friend difference_type
      operator-(std::default_sentinel_t, __iterator __it) noexcept
      { return difference_type(_Vp::size.value) - __it._M_offset; }
    };
} // namespace simd
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif // C++26
#endif // _GLIBCXX_SIMD_ITERATOR_H
