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

#ifndef _GLIBCXX_SIMD_MASK_REDUCTIONS_H
#define _GLIBCXX_SIMD_MASK_REDUCTIONS_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#if __cplusplus >= 202400L

#include "simd_mask.h"

// psabi warnings are bogus because the ABI of the internal types never leaks into user code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi"

// [simd.mask.reductions] -----------------------------------------------------
namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace simd
{
  template <size_t _Bytes, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr bool
    all_of(const basic_mask<_Bytes, _Ap>& __k) noexcept
    { return __k._M_all_of(); }

  template <size_t _Bytes, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr bool
    any_of(const basic_mask<_Bytes, _Ap>& __k) noexcept
    { return __k._M_any_of(); }

  template <size_t _Bytes, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr bool
    none_of(const basic_mask<_Bytes, _Ap>& __k) noexcept
    { return __k._M_none_of(); }

  template <size_t _Bytes, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr __simd_size_type
    reduce_count(const basic_mask<_Bytes, _Ap>& __k) noexcept
    {
      if constexpr (_Ap::_S_size == 1)
	return +__k[0];
      else if constexpr (_Ap::_S_is_vecmask)
	return -reduce(-__k);
      else
	return __k._M_reduce_count();
    }

  template <size_t _Bytes, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr __simd_size_type
    reduce_min_index(const basic_mask<_Bytes, _Ap>& __k)
    { return __k._M_reduce_min_index(); }

  template <size_t _Bytes, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr __simd_size_type
    reduce_max_index(const basic_mask<_Bytes, _Ap>& __k)
    { return __k._M_reduce_max_index(); }

  constexpr bool
  all_of(same_as<bool> auto __x) noexcept
  { return __x; }

  constexpr bool
  any_of(same_as<bool> auto __x) noexcept
  { return __x; }

  constexpr bool
  none_of(same_as<bool> auto __x) noexcept
  { return !__x; }

  constexpr __simd_size_type
  reduce_count(same_as<bool> auto __x) noexcept
  { return __x; }

  constexpr __simd_size_type
  reduce_min_index(same_as<bool> auto __x)
  { return 0; }

  constexpr __simd_size_type
  reduce_max_index(same_as<bool> auto __x)
  { return 0; }
} // namespace simd
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#pragma GCC diagnostic pop
#endif // C++26
#endif // _GLIBCXX_SIMD_MASK_REDUCTIONS_H
