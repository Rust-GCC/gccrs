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

#ifndef _GLIBCXX_SIMD_FLAGS_H
#define _GLIBCXX_SIMD_FLAGS_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#if __cplusplus >= 202400L

#include "simd_details.h"
#include <bits/align.h> // assume_aligned

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace simd
{
  // [simd.traits]
  // --- alignment ---
  template <typename _Tp, typename _Up = typename _Tp::value_type>
    struct alignment
    {};

  template <typename _Tp, typename _Ap, __vectorizable _Up>
    struct alignment<basic_vec<_Tp, _Ap>, _Up>
    : integral_constant<size_t, alignof(basic_vec<_Tp, _Ap>)>
    {};

  template <typename _Tp, typename _Up = typename _Tp::value_type>
    constexpr size_t alignment_v = alignment<_Tp, _Up>::value;

  // [simd.flags] -------------------------------------------------------------
  struct _LoadStoreTag
  {};

  /** @internal
   * `struct convert-flag`
   *
   * C++26 [simd.expos] / [simd.flags]
   */
  struct __convert_flag
  : _LoadStoreTag
  {};

  /** @internal
   * `struct aligned-flag`
   *
   * C++26 [simd.expos] / [simd.flags]
   */
  struct __aligned_flag
  : _LoadStoreTag
  {
    template <typename _Tp, typename _Up>
      [[__gnu__::__always_inline__]]
      static constexpr _Up*
      _S_adjust_pointer(_Up* __ptr)
      { return assume_aligned<simd::alignment_v<_Tp, remove_cv_t<_Up>>>(__ptr); }
  };

  /** @internal
   * `template<size_t N> struct overaligned-flag`
   *
   * @tparam _Np  alignment in bytes
   *
   * C++26 [simd.expos] / [simd.flags]
   */
  template <size_t _Np>
    struct __overaligned_flag
    : _LoadStoreTag
    {
      static_assert(__has_single_bit(_Np));

      template <typename, typename _Up>
	[[__gnu__::__always_inline__]]
	static constexpr _Up*
	_S_adjust_pointer(_Up* __ptr)
	{ return assume_aligned<_Np>(__ptr); }
    };

  struct __partial_loadstore_flag
  : _LoadStoreTag
  {};


  template <typename _Tp>
    concept __loadstore_tag = is_base_of_v<_LoadStoreTag, _Tp>;

  template <typename...>
    struct flags;

  template <typename... _Flags>
    requires (__loadstore_tag<_Flags> && ...)
    struct flags<_Flags...>
    {
      /** @internal
       * Returns @c true if the given argument is part of this specialization, otherwise returns @c
       * false.
       */
      template <typename _F0>
	static consteval bool
	_S_test(flags<_F0>)
	{ return (is_same_v<_Flags, _F0> || ...); }

      friend consteval flags
      operator|(flags, flags<>)
      { return flags{}; }

      template <typename _T0, typename... _More>
	friend consteval auto
	operator|(flags, flags<_T0, _More...>)
	{
	  if constexpr ((same_as<_Flags, _T0> || ...))
	    return flags<_Flags...>{} | flags<_More...>{};
	  else
	    return flags<_Flags..., _T0>{} | flags<_More...>{};
	}

      /** @internal
       * Adjusts a pointer according to the alignment requirements of the flags.
       *
       * This function iterates over all flags in the pack and applies each flag's
       * `_S_adjust_pointer` method to the input pointer. Flags that don't provide
       * this method are ignored.
       *
       * @tparam _Tp  A basic_vec type for which a load/store pointer is adjusted
       * @tparam _Up  The value-type of the input/output range
       * @param __ptr  The pointer to the range
       * @return The adjusted pointer
       */
      template <typename _Tp, typename _Up>
	static constexpr _Up*
	_S_adjust_pointer(_Up* __ptr)
	{
	  template for ([[maybe_unused]] constexpr auto __f : {_Flags()...})
	    {
	      if constexpr (requires {__f.template _S_adjust_pointer<_Tp>(__ptr); })
		__ptr = __f.template _S_adjust_pointer<_Tp>(__ptr);
	    }
	  return __ptr;
	}
    };

  inline constexpr flags<> flag_default {};

  inline constexpr flags<__convert_flag> flag_convert {};

  inline constexpr flags<__aligned_flag> flag_aligned {};

  template <size_t _Np>
    requires(__has_single_bit(_Np))
    inline constexpr flags<__overaligned_flag<_Np>> flag_overaligned {};

  /** @internal
   * Pass to unchecked_load or unchecked_store to make it behave like partial_load / partial_store.
   */
  inline constexpr flags<__partial_loadstore_flag> __allow_partial_loadstore {};

} // namespace simd
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif // C++26
#endif // _GLIBCXX_SIMD_FLAGS_H
