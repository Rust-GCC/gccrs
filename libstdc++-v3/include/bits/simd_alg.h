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

#ifndef _GLIBCXX_SIMD_ALG_H
#define _GLIBCXX_SIMD_ALG_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#if __cplusplus >= 202400L

#include "simd_vec.h"

// psabi warnings are bogus because the ABI of the internal types never leaks into user code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi"

// [simd.alg] -----------------------------------------------------------------
namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace simd
{
  template<typename _Tp, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr basic_vec<_Tp, _Ap>
    min(const basic_vec<_Tp, _Ap>& __a, const basic_vec<_Tp, _Ap>& __b) noexcept
    { return __select_impl(__a < __b, __a, __b); }

  template<typename _Tp, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr basic_vec<_Tp, _Ap>
    max(const basic_vec<_Tp, _Ap>& __a, const basic_vec<_Tp, _Ap>& __b) noexcept
    { return __select_impl(__a < __b, __b, __a); }

  template<typename _Tp, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr pair<basic_vec<_Tp, _Ap>, basic_vec<_Tp, _Ap>>
    minmax(const basic_vec<_Tp, _Ap>& __a, const basic_vec<_Tp, _Ap>& __b) noexcept
    { return {min(__a, __b), max(__a, __b)}; }

  template<typename _Tp, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr basic_vec<_Tp, _Ap>
    clamp(const basic_vec<_Tp, _Ap>& __v, const basic_vec<_Tp, _Ap>& __lo,
	  const basic_vec<_Tp, _Ap>& __hi)
    {
      __glibcxx_simd_precondition(none_of(__lo > __hi), "lower bound is larger than upper bound");
      return max(__lo, min(__hi, __v));
    }

  template<typename _Tp, typename _Up>
    constexpr auto
    select(bool __c, const _Tp& __a, const _Up& __b)
    -> remove_cvref_t<decltype(__c ? __a : __b)>
    { return __c ? __a : __b; }

  template<size_t _Bytes, typename _Ap, typename _Tp, typename _Up>
    [[__gnu__::__always_inline__]]
    constexpr auto
    select(const basic_mask<_Bytes, _Ap>& __c, const _Tp& __a, const _Up& __b)
    noexcept -> decltype(__select_impl(__c, __a, __b))
    { return __select_impl(__c, __a, __b); }
} // namespace simd

  using simd::min;
  using simd::max;
  using simd::minmax;
  using simd::clamp;

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#pragma GCC diagnostic pop
#endif // C++26
#endif // _GLIBCXX_SIMD_ALG_H
