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

#ifndef _GLIBCXX_SIMD_LOADSTORE_H
#define _GLIBCXX_SIMD_LOADSTORE_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#if __cplusplus >= 202400L

#include "simd_vec.h"

// psabi warnings are bogus because the ABI of the internal types never leaks into user code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi"

// [simd.reductions] ----------------------------------------------------------
namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace simd
{
  template <typename _Vp, typename _Tp>
    struct __vec_load_return
    { using type = _Vp; };

  template <typename _Tp>
    struct __vec_load_return<void, _Tp>
    { using type = basic_vec<_Tp>; };

  template <typename _Vp, typename _Tp>
    using __vec_load_return_t = typename __vec_load_return<_Vp, _Tp>::type;

  template <typename _Vp, typename _Tp>
    using __load_mask_type_t = typename __vec_load_return_t<_Vp, _Tp>::mask_type;

  template <typename _Tp>
    concept __sized_contiguous_range
      = ranges::contiguous_range<_Tp> && ranges::sized_range<_Tp>;

  template <typename _Vp = void, __sized_contiguous_range _Rg, typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, ranges::range_value_t<_Rg>>
    unchecked_load(_Rg&& __r, flags<_Flags...> __f = {})
    {
      using _Tp = ranges::range_value_t<_Rg>;
      using _RV = __vec_load_return_t<_Vp, _Tp>;
      using _Rp = typename _RV::value_type;
      static_assert(__loadstore_convertible_to<ranges::range_value_t<_Rg>, _Rp, _Flags...>,
		    "'flag_convert' must be used for conversions that are not value-preserving");

      constexpr bool __allow_out_of_bounds = __f._S_test(__allow_partial_loadstore);
      constexpr size_t __static_size = __static_range_size(__r);

      if constexpr (!__allow_out_of_bounds && __static_sized_range<_Rg>)
	static_assert(ranges::size(__r) >= _RV::size(), "given range must have sufficient size");

      const auto* __ptr = __f.template _S_adjust_pointer<_RV>(ranges::data(__r));
      const auto __rg_size = std::ranges::size(__r);
      if constexpr (!__allow_out_of_bounds)
	__glibcxx_simd_precondition(
	  std::ranges::size(__r) >= _RV::size(),
	  "Input range is too small. Did you mean to use 'partial_load'?");

      if consteval
	{
	  return _RV([&](size_t __i) -> _Rp {
		   if (__i >= __rg_size)
		     return _Rp();
		   else
		     return static_cast<_Rp>(__r[__i]);
		 });
	}
      else
	{
	  if constexpr ((__static_size != dynamic_extent && __static_size >= size_t(_RV::size()))
			  || !__allow_out_of_bounds)
	    return _RV(_LoadCtorTag(), __ptr);
	  else
	    return _RV::_S_partial_load(__ptr, __rg_size);
	}
    }

  template <typename _Vp = void, __sized_contiguous_range _Rg, typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, ranges::range_value_t<_Rg>>
    unchecked_load(_Rg&& __r, const __load_mask_type_t<_Vp, ranges::range_value_t<_Rg>>& __mask,
		   flags<_Flags...> __f = {})
    {
      using _Tp = ranges::range_value_t<_Rg>;
      using _RV = __vec_load_return_t<_Vp, _Tp>;
      using _Rp = typename _RV::value_type;
      static_assert(__vectorizable<_Tp>);
      static_assert(__explicitly_convertible_to<_Tp, _Rp>);
      static_assert(__loadstore_convertible_to<_Tp, _Rp, _Flags...>,
		    "'flag_convert' must be used for conversions that are not value-preserving");

      constexpr bool __allow_out_of_bounds = __f._S_test(__allow_partial_loadstore);
      constexpr auto __static_size = __static_range_size(__r);

      if constexpr (!__allow_out_of_bounds && __static_sized_range<_Rg>)
	static_assert(ranges::size(__r) >= _RV::size(), "given range must have sufficient size");

      const auto* __ptr = __f.template _S_adjust_pointer<_RV>(ranges::data(__r));

      if constexpr (!__allow_out_of_bounds)
	__glibcxx_simd_precondition(
	  ranges::size(__r) >= size_t(_RV::size()),
	  "Input range is too small. Did you mean to use 'partial_load'?");

      const size_t __rg_size = ranges::size(__r);
      if consteval
	{
	  return _RV([&](size_t __i) -> _Rp {
		   if (__i >= __rg_size || !__mask[int(__i)])
		     return _Rp();
		   else
		     return static_cast<_Rp>(__r[__i]);
		 });
	}
      else
	{
	  constexpr bool __no_size_check
	    = !__allow_out_of_bounds
		|| (__static_size != dynamic_extent
		      && __static_size >= size_t(_RV::size.value));
	  if constexpr (_RV::size() == 1)
	    return __mask[0] && (__no_size_check || __rg_size > 0) ? _RV(_LoadCtorTag(), __ptr)
								   : _RV();
	  else if constexpr (__no_size_check)
	    return _RV::_S_masked_load(__ptr, __mask);
	  else if (__rg_size >= size_t(_RV::size()))
	    return _RV::_S_masked_load(__ptr, __mask);
	  else if (__rg_size > 0)
	    return _RV::_S_masked_load(
		     __ptr, __mask && _RV::mask_type::_S_partial_mask_of_n(int(__rg_size)));
	  else
	    return _RV();
	}
    }

  template <typename _Vp = void, contiguous_iterator _It, typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, iter_value_t<_It>>
    unchecked_load(_It __first, iter_difference_t<_It> __n, flags<_Flags...> __f = {})
    { return simd::unchecked_load<_Vp>(span<const iter_value_t<_It>>(__first, __n), __f); }

  template <typename _Vp = void, contiguous_iterator _It, typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, iter_value_t<_It>>
    unchecked_load(_It __first, iter_difference_t<_It> __n,
		   const __load_mask_type_t<_Vp, iter_value_t<_It>>& __mask,
		   flags<_Flags...> __f = {})
    { return simd::unchecked_load<_Vp>(span<const iter_value_t<_It>>(__first, __n), __mask, __f); }

  template <typename _Vp = void, contiguous_iterator _It, sized_sentinel_for<_It> _Sp,
	    typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, iter_value_t<_It>>
    unchecked_load(_It __first, _Sp __last, flags<_Flags...> __f = {})
    { return simd::unchecked_load<_Vp>(span<const iter_value_t<_It>>(__first, __last), __f); }

  template <typename _Vp = void, contiguous_iterator _It, sized_sentinel_for<_It> _Sp,
	    typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, iter_value_t<_It>>
    unchecked_load(_It __first, _Sp __last,
		   const __load_mask_type_t<_Vp, iter_value_t<_It>>& __mask,
		   flags<_Flags...> __f = {})
    {
      return simd::unchecked_load<_Vp>(span<const iter_value_t<_It>>(__first, __last), __mask, __f);
    }

  template <typename _Vp = void, __sized_contiguous_range _Rg, typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, ranges::range_value_t<_Rg>>
    partial_load(_Rg&& __r, flags<_Flags...> __f = {})
    { return simd::unchecked_load<_Vp>(__r, __f | __allow_partial_loadstore); }

  template <typename _Vp = void, __sized_contiguous_range _Rg, typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, ranges::range_value_t<_Rg>>
    partial_load(_Rg&& __r, const __load_mask_type_t<_Vp, ranges::range_value_t<_Rg>>& __mask,
		 flags<_Flags...> __f = {})
    { return simd::unchecked_load<_Vp>(__r, __mask, __f | __allow_partial_loadstore); }

  template <typename _Vp = void, contiguous_iterator _It, typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, iter_value_t<_It>>
    partial_load(_It __first, iter_difference_t<_It> __n, flags<_Flags...> __f = {})
    { return partial_load<_Vp>(span<const iter_value_t<_It>>(__first, __n), __f); }

  template <typename _Vp = void, contiguous_iterator _It, typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, iter_value_t<_It>>
    partial_load(_It __first, iter_difference_t<_It> __n,
		 const __load_mask_type_t<_Vp, iter_value_t<_It>>& __mask,
		 flags<_Flags...> __f = {})
    { return partial_load<_Vp>(span<const iter_value_t<_It>>(__first, __n), __mask, __f); }

  template <typename _Vp = void, contiguous_iterator _It, sized_sentinel_for<_It> _Sp,
	    typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, iter_value_t<_It>>
    partial_load(_It __first, _Sp __last, flags<_Flags...> __f = {})
    { return partial_load<_Vp>(span<const iter_value_t<_It>>(__first, __last), __f); }

  template <typename _Vp = void, contiguous_iterator _It, sized_sentinel_for<_It> _Sp,
	    typename... _Flags>
    [[__gnu__::__always_inline__]]
    constexpr __vec_load_return_t<_Vp, iter_value_t<_It>>
    partial_load(_It __first, _Sp __last, const __load_mask_type_t<_Vp, iter_value_t<_It>>& __mask,
		 flags<_Flags...> __f = {})
    { return partial_load<_Vp>(span<const iter_value_t<_It>>(__first, __last), __mask, __f); }

  template <typename _Tp, typename _Ap, __sized_contiguous_range _Rg, typename... _Flags>
    requires indirectly_writable<ranges::iterator_t<_Rg>, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    unchecked_store(const basic_vec<_Tp, _Ap>& __v, _Rg&& __r, flags<_Flags...> __f = {})
    {
      using _TV = basic_vec<_Tp, _Ap>;
      static_assert(destructible<_TV>);
      static_assert(__loadstore_convertible_to<_Tp, ranges::range_value_t<_Rg>, _Flags...>,
		    "'flag_convert' must be used for conversions that are not value-preserving");

      constexpr bool __allow_out_of_bounds = __f._S_test(__allow_partial_loadstore);
      if constexpr (!__allow_out_of_bounds && __static_sized_range<_Rg>)
	static_assert(ranges::size(__r) >= _TV::size(), "given range must have sufficient size");

      auto* __ptr = __f.template _S_adjust_pointer<_TV>(ranges::data(__r));
      const auto __rg_size = ranges::size(__r);
      if constexpr (!__allow_out_of_bounds)
	__glibcxx_simd_precondition(
	  ranges::size(__r) >= _TV::size(),
	  "output range is too small. Did you mean to use 'partial_store'?");

      if consteval
	{
	  for (unsigned __i = 0; __i < __rg_size && __i < _TV::size(); ++__i)
	    __ptr[__i] = static_cast<ranges::range_value_t<_Rg>>(__v[__i]);
	}
      else
	{
	  if constexpr (!__allow_out_of_bounds)
	    __v._M_store(__ptr);
	  else
	    _TV::_S_partial_store(__v, __ptr, __rg_size);
	}
    }

  template <typename _Tp, typename _Ap, __sized_contiguous_range _Rg, typename... _Flags>
    requires indirectly_writable<ranges::iterator_t<_Rg>, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    unchecked_store(const basic_vec<_Tp, _Ap>& __v, _Rg&& __r,
		    const typename basic_vec<_Tp, _Ap>::mask_type& __mask,
		    flags<_Flags...> __f = {})
    {
      using _TV = basic_vec<_Tp, _Ap>;
      static_assert(__loadstore_convertible_to<_Tp, ranges::range_value_t<_Rg>, _Flags...>,
		    "'flag_convert' must be used for conversions that are not value-preserving");

      constexpr bool __allow_out_of_bounds = __f._S_test(__allow_partial_loadstore);
      if constexpr (!__allow_out_of_bounds && __static_sized_range<_Rg>)
	static_assert(ranges::size(__r) >= _TV::size(), "given range must have sufficient size");

      auto* __ptr = __f.template _S_adjust_pointer<_TV>(ranges::data(__r));

      if constexpr (!__allow_out_of_bounds)
	__glibcxx_simd_precondition(
	  ranges::size(__r) >= size_t(_TV::size()),
	  "output range is too small. Did you mean to use 'partial_store'?");

      const size_t __rg_size = ranges::size(__r);
      if consteval
	{
	  for (int __i = 0; __i < _TV::size(); ++__i)
	    {
	      if (__mask[__i] && (!__allow_out_of_bounds || size_t(__i) < __rg_size))
		__ptr[__i] = static_cast<ranges::range_value_t<_Rg>>(__v[__i]);
	    }
	}
      else
	{
	  if (__allow_out_of_bounds && __rg_size < size_t(_TV::size()))
	    _TV::_S_masked_store(__v, __ptr,
				 __mask && _TV::mask_type::_S_partial_mask_of_n(int(__rg_size)));
	  else
	    _TV::_S_masked_store(__v, __ptr, __mask);
	}
    }

  template <typename _Tp, typename _Ap, contiguous_iterator _It, typename... _Flags>
    requires indirectly_writable<_It, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    unchecked_store(const basic_vec<_Tp, _Ap>& __v, _It __first,
		    iter_difference_t<_It> __n, flags<_Flags...> __f = {})
    { simd::unchecked_store(__v, std::span<iter_value_t<_It>>(__first, __n), __f); }

  template <typename _Tp, typename _Ap, contiguous_iterator _It, typename... _Flags>
    requires indirectly_writable<_It, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    unchecked_store(const basic_vec<_Tp, _Ap>& __v, _It __first, iter_difference_t<_It> __n,
		    const typename basic_vec<_Tp, _Ap>::mask_type& __mask,
		    flags<_Flags...> __f = {})
    { simd::unchecked_store(__v, std::span<iter_value_t<_It>>(__first, __n), __mask, __f); }

  template <typename _Tp, typename _Ap, contiguous_iterator _It, sized_sentinel_for<_It> _Sp,
	    typename... _Flags>
    requires indirectly_writable<_It, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    unchecked_store(const basic_vec<_Tp, _Ap>& __v, _It __first, _Sp __last,
		    flags<_Flags...> __f = {})
    { simd::unchecked_store(__v, std::span<iter_value_t<_It>>(__first, __last), __f); }

  template <typename _Tp, typename _Ap, contiguous_iterator _It, sized_sentinel_for<_It> _Sp,
	    typename... _Flags>
    requires indirectly_writable<_It, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    unchecked_store(const basic_vec<_Tp, _Ap>& __v, _It __first, _Sp __last,
		    const typename basic_vec<_Tp, _Ap>::mask_type& __mask,
		    flags<_Flags...> __f = {})
    { simd::unchecked_store(__v, std::span<iter_value_t<_It>>(__first, __last), __mask, __f); }

  template <typename _Tp, typename _Ap, __sized_contiguous_range _Rg, typename... _Flags>
    requires indirectly_writable<ranges::iterator_t<_Rg>, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    partial_store(const basic_vec<_Tp, _Ap>& __v, _Rg&& __r, flags<_Flags...> __f = {})
    { simd::unchecked_store(__v, __r, __f | __allow_partial_loadstore); }

  template <typename _Tp, typename _Ap, __sized_contiguous_range _Rg, typename... _Flags>
    requires indirectly_writable<ranges::iterator_t<_Rg>, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    partial_store(const basic_vec<_Tp, _Ap>& __v, _Rg&& __r,
		  const typename basic_vec<_Tp, _Ap>::mask_type& __mask,
		  flags<_Flags...> __f = {})
    { simd::unchecked_store(__v, __r, __mask, __f | __allow_partial_loadstore); }

  template <typename _Tp, typename _Ap, contiguous_iterator _It, typename... _Flags>
    requires indirectly_writable<_It, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    partial_store(const basic_vec<_Tp, _Ap>& __v, _It __first, iter_difference_t<_It> __n,
		  flags<_Flags...> __f = {})
    { partial_store(__v, span(__first, __n), __f); }

  template <typename _Tp, typename _Ap, contiguous_iterator _It, typename... _Flags>
    requires indirectly_writable<_It, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    partial_store(const basic_vec<_Tp, _Ap>& __v, _It __first, iter_difference_t<_It> __n,
		  const typename basic_vec<_Tp, _Ap>::mask_type& __mask, flags<_Flags...> __f = {})
    { partial_store(__v, span(__first, __n), __mask, __f); }

  template <typename _Tp, typename _Ap, contiguous_iterator _It, sized_sentinel_for<_It> _Sp,
	    typename... _Flags>
    requires indirectly_writable<_It, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    partial_store(const basic_vec<_Tp, _Ap>& __v, _It __first, _Sp __last,
		  flags<_Flags...> __f = {})
    { partial_store(__v, span(__first, __last), __f); }

  template <typename _Tp, typename _Ap, contiguous_iterator _It, sized_sentinel_for<_It> _Sp,
	    typename... _Flags>
    requires indirectly_writable<_It, _Tp>
    [[__gnu__::__always_inline__]]
    constexpr void
    partial_store(const basic_vec<_Tp, _Ap>& __v, _It __first, _Sp __last,
		  const typename basic_vec<_Tp, _Ap>::mask_type& __mask, flags<_Flags...> __f = {})
    { partial_store(__v, span(__first, __last), __mask, __f); }
} // namespace simd
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#pragma GCC diagnostic pop
#endif // C++26
#endif // _GLIBCXX_SIMD_LOADSTORE_H
