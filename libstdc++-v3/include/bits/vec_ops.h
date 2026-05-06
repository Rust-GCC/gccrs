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

#ifndef _GLIBCXX_VEC_OPS_H
#define _GLIBCXX_VEC_OPS_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#if __cplusplus >= 202400L

#include "simd_details.h"

#include <bit>
#include <bits/utility.h>

// psabi warnings are bogus because the ABI of the internal types never leaks into user code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi"

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace simd
{
  template <std::signed_integral _Tp>
    constexpr bool
    __signed_has_single_bit(_Tp __x)
    { return __has_single_bit(make_unsigned_t<_Tp>(__x)); }

  /**
   * Alias for a vector builtin with given value type and total sizeof.
   */
  template <__vectorizable _Tp, size_t _Bytes>
    requires (__has_single_bit(_Bytes))
    using __vec_builtin_type_bytes [[__gnu__::__vector_size__(_Bytes)]] = _Tp;

  /**
   * Alias for a vector builtin with given value type @p _Tp and @p _Width.
   */
  template <__vectorizable _Tp, __simd_size_type _Width>
    requires (__signed_has_single_bit(_Width))
    using __vec_builtin_type = __vec_builtin_type_bytes<_Tp, sizeof(_Tp) * _Width>;

  /**
   * Constrain to any vector builtin with given value type and optional width.
   */
  template <typename _Tp, typename _ValueType,
	    __simd_size_type _Width = sizeof(_Tp) / sizeof(_ValueType)>
    concept __vec_builtin_of
      = !is_class_v<_Tp> && !is_pointer_v<_Tp> && !is_arithmetic_v<_Tp>
	  && __vectorizable<_ValueType>
	  && _Width >= 1 && sizeof(_Tp) / sizeof(_ValueType) == _Width
	  && same_as<__vec_builtin_type_bytes<_ValueType, sizeof(_Tp)>, _Tp>
	  && requires(_Tp& __v, _ValueType __x) { __v[0] = __x; };

  /**
   * Constrain to any vector builtin.
   */
  template <typename _Tp>
    concept __vec_builtin
      = __vec_builtin_of<_Tp, remove_cvref_t<decltype(declval<const _Tp>()[0])>>;

  /**
   * Alias for the value type of the given __vec_builtin type @p _Tp.
   */
  template <__vec_builtin _Tp>
    using __vec_value_type = remove_cvref_t<decltype(declval<const _Tp>()[0])>;

  /**
   * The width (number of value_type elements) of the given vector builtin or arithmetic type.
   */
  template <typename _Tp>
    inline constexpr __simd_size_type __width_of = 1;

  template <typename _Tp>
    requires __vec_builtin<_Tp>
    inline constexpr __simd_size_type __width_of<_Tp> = sizeof(_Tp) / sizeof(__vec_value_type<_Tp>);

  /**
   * Alias for a vector builtin with equal value type and new width @p _Np.
   */
  template <__simd_size_type _Np, __vec_builtin _TV>
    using __resize_vec_builtin_t = __vec_builtin_type<__vec_value_type<_TV>, _Np>;

  template <__vec_builtin _TV>
    requires (__width_of<_TV> > 1)
    using __half_vec_builtin_t = __resize_vec_builtin_t<__width_of<_TV> / 2, _TV>;

  template <__vec_builtin _TV>
    using __double_vec_builtin_t = __resize_vec_builtin_t<__width_of<_TV> * 2, _TV>;

  template <typename _Up, __vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr __vec_builtin_type_bytes<_Up, sizeof(_TV)>
    __vec_bit_cast(_TV __v)
    { return reinterpret_cast<__vec_builtin_type_bytes<_Up, sizeof(_TV)>>(__v); }

  template <int _Np, __vec_builtin _TV>
    requires signed_integral<__vec_value_type<_TV>>
    static constexpr _TV _S_vec_implicit_mask = []<int... _Is> (integer_sequence<int, _Is...>) {
      return _TV{ (_Is < _Np ? -1 : 0)... };
    } (make_integer_sequence<int, __width_of<_TV>>());

  /**
   * Helper function to work around Clang not allowing v[i] in constant expressions.
   */
  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr __vec_value_type<_TV>
    __vec_get(_TV __v, int __i)
    {
#ifdef _GLIBCXX_CLANG
      if consteval
	{
	  return __builtin_bit_cast(array<__vec_value_type<_TV>, __width_of<_TV>>, __v)[__i];
	}
      else
#endif
	{
	  return __v[__i];
	}
    }

  /**
   * Helper function to work around Clang and GCC not allowing assignment to v[i] in constant
   * expressions.
   */
  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr void
    __vec_set(_TV& __v, int __i, __vec_value_type<_TV> __x)
    {
      if consteval
	{
#ifdef _GLIBCXX_CLANG
	  auto __arr = __builtin_bit_cast(array<__vec_value_type<_TV>, __width_of<_TV>>, __v);
	  __arr[__i] = __x;
	  __v = __builtin_bit_cast(_TV, __arr);
#else
	  constexpr auto [...__j] = _IotaArray<__width_of<_TV>>;
	  __v = _TV{(__i == __j ? __x : __v[__j])...};
#endif
	}
      else
	{
	  __v[__i] = __x;
	}
    }

  /** @internal
   * Return vector builtin with all values from @p __a and @p __b.
   */
  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr __vec_builtin_type<__vec_value_type<_TV>, __width_of<_TV> * 2>
    __vec_concat(_TV __a, _TV __b)
    {
      constexpr auto [...__is] = _IotaArray<__width_of<_TV> * 2>;
      return __builtin_shufflevector(__a, __b, __is...);
    }

  /** @internal
   * Concatenate the first @p _N0 elements from @p __a with the first @p _N1 elements from @p __b
   * with the elements from applying this function recursively to @p __rest.
   *
   * @pre _N0 <= __width_of<_TV0> && _N1 <= __width_of<_TV1> && _Ns <= __width_of<_TVs> && ...
   *
   * Strategy: Aim for a power-of-2 tree concat. E.g.
   * - cat(2, 2, 2, 2) -> cat(4, 2, 2) -> cat(4, 4)
   * - cat(2, 2, 2, 2, 8) -> cat(4, 2, 2, 8) -> cat(4, 4, 8) -> cat(8, 8)
   */
  template <int _N0, int _N1, int... _Ns, __vec_builtin _TV0, __vec_builtin _TV1,
	   __vec_builtin... _TVs>
    [[__gnu__::__always_inline__]]
    constexpr __vec_builtin_type<__vec_value_type<_TV0>,
				 __bit_ceil(unsigned(_N0 + (_N1 + ... + _Ns)))>
    __vec_concat_sized(const _TV0& __a, const _TV1& __b, const _TVs&... __rest);

  template <int _N0, int _N1, int _N2, int... _Ns, __vec_builtin _TV0, __vec_builtin _TV1,
	    __vec_builtin _TV2, __vec_builtin... _TVs>
    requires (__has_single_bit(unsigned(_N0))) && (_N0 >= (_N1 + _N2))
    [[__gnu__::__always_inline__]]
    constexpr __vec_builtin_type<__vec_value_type<_TV0>,
				 __bit_ceil(unsigned(_N0 + _N1 + (_N2 + ... + _Ns)))>
    __vec_concat_sized(const _TV0& __a, const _TV1& __b, const _TV2& __c, const _TVs&... __rest)
    {
      return __vec_concat_sized<_N0, _N1 + _N2, _Ns...>(
	       __a, __vec_concat_sized<_N1, _N2>(__b, __c), __rest...);
    }

  template <int _N0, int _N1, int... _Ns, __vec_builtin _TV0, __vec_builtin _TV1,
	   __vec_builtin... _TVs>
    [[__gnu__::__always_inline__]]
    constexpr __vec_builtin_type<__vec_value_type<_TV0>,
				 __bit_ceil(unsigned(_N0 + (_N1 + ... + _Ns)))>
    __vec_concat_sized(const _TV0& __a, const _TV1& __b, const _TVs&... __rest)
    {
      // __is is rounded up because we need to generate a power-of-2 vector:
      constexpr auto [...__is] = _IotaArray<__bit_ceil(unsigned(_N0 + _N1)), int>;
      const auto __ab = __builtin_shufflevector(__a, __b, [](int __i) consteval {
			  if (__i < _N0) // copy from __a
			    return __i;
			  else if (__i < _N0 + _N1) // copy from __b
			    return __i - _N0 + __width_of<_TV0>; // _N0 <= __width_of<_TV0>
			  else // can't index into __rest
			    return -1; // don't care
			}(__is)...);
      if constexpr (sizeof...(__rest) == 0)
	return __ab;
      else
	return __vec_concat_sized<_N0 + _N1, _Ns...>(__ab, __rest...);
    }

  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr __half_vec_builtin_t<_TV>
    __vec_split_lo(_TV __v)
    {
      constexpr int __n = __width_of<_TV> / 2;
      constexpr auto [...__is] = _IotaArray<__n>;
      return __builtin_shufflevector(__v, __v, __is...);
    }

  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr __half_vec_builtin_t<_TV>
    __vec_split_hi(_TV __v)
    {
      constexpr int __n = __width_of<_TV> / 2;
      constexpr auto [...__is] = _IotaArray<__n>;
      return __builtin_shufflevector(__v, __v, (__n + __is)...);
    }

  /** @internal
   * Return @p __x zero-padded to @p _Bytes bytes.
   *
   * Use this function when you need two objects of the same size (e.g. for __vec_concat).
   */
  template <size_t _Bytes, __vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr auto
    __vec_zero_pad_to(_TV __x)
    {
      if constexpr (sizeof(_TV) == _Bytes)
	return __x;
      else if constexpr (sizeof(_TV) <= sizeof(0ull))
	{
	  using _Up = _UInt<sizeof(_TV)>;
	  __vec_builtin_type_bytes<_Up, _Bytes> __tmp = {__builtin_bit_cast(_Up, __x)};
	  return __builtin_bit_cast(__vec_builtin_type_bytes<__vec_value_type<_TV>, _Bytes>, __tmp);
	}
      else if constexpr (sizeof(_TV) < _Bytes)
	return __vec_zero_pad_to<_Bytes>(__vec_concat(__x, _TV()));
      else
	static_assert(false);
    }

  /** @internal
   * Return a type with sizeof 16, add zero-padding to @p __x. The input must be smaller.
   *
   * Use this function instead of the above when you need to pad an argument for a SIMD builtin.
   */
  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr auto
    __vec_zero_pad_to_16(_TV __x)
    {
      static_assert(sizeof(_TV) < 16);
      return __vec_zero_pad_to<16>(__x);
    }

  // work around __builtin_constant_p returning false unless passed a variable
  // (__builtin_constant_p(x[0]) is false while __is_const_known(x[0]) is true)
  template <typename _Tp>
    [[__gnu__::__always_inline__]]
    constexpr bool
    __is_const_known(const _Tp& __x)
    {
      return __builtin_constant_p(__x);
    }

  [[__gnu__::__always_inline__]]
  constexpr bool
  __is_const_known(const auto&... __xs) requires(sizeof...(__xs) >= 2)
  {
    if consteval
      {
	return true;
      }
    else
      {
	return (__is_const_known(__xs) && ...);
      }
  }

  [[__gnu__::__always_inline__]]
  constexpr bool
  __is_const_known_equal_to(const auto& __x, const auto& __expect)
  { return __is_const_known(__x == __expect) && __x == __expect; }

#if _GLIBCXX_X86
  template <__vec_builtin _UV, __vec_builtin _TV>
    inline _UV
    __x86_cvt_f16c(_TV __v);
#endif


  /** @internal
   * Simple wrapper around __builtin_convertvector to provide static_cast-like syntax.
   *
   * Works around GCC failing to use the F16C/AVX512F cvtps2ph/cvtph2ps instructions.
   */
  template <__vec_builtin _UV, __vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    constexpr _UV
    __vec_cast(_TV __v)
    {
      static_assert(__width_of<_UV> == __width_of<_TV>);
#if _GLIBCXX_X86
      using _Up = __vec_value_type<_UV>;
      using _Tp = __vec_value_type<_TV>;
      constexpr bool __to_f16 = is_same_v<_Up, _Float16>;
      constexpr bool __from_f16 = is_same_v<_Tp, _Float16>;
      constexpr bool __needs_f16c = _Traits._M_have_f16c() && !_Traits._M_have_avx512fp16()
				      && (__to_f16 || __from_f16);
      if (__needs_f16c && !__is_const_known(__v))
	{ // Work around PR121688
	  if constexpr (__needs_f16c)
	    return __x86_cvt_f16c<_UV>(__v);
	}
      if constexpr (is_floating_point_v<_Tp> && is_integral_v<_Up>
		      && sizeof(_UV) < sizeof(_TV) && sizeof(_Up) < sizeof(int))
	{
	  using _Ip = __integer_from<std::min(sizeof(int), sizeof(_Tp))>;
	  using _IV = __vec_builtin_type<_Ip, __width_of<_TV>>;
	  return __vec_cast<_UV>(__vec_cast<_IV>(__v));
	}
#endif
      return __builtin_convertvector(__v, _UV);
    }

  /** @internal
   * Overload of the above cast function that determines the destination vector type from a given
   * element type @p _Up and the `__width_of` the argument type.
   *
   * Calls the above overload.
   */
  template <__vectorizable _Up, __vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr __vec_builtin_type<_Up, __width_of<_TV>>
    __vec_cast(_TV __v)
    { return __vec_cast<__vec_builtin_type<_Up, __width_of<_TV>>>(__v); }

  /** @internal
   * As above, but with additional precondition on possible values of the argument.
   *
   * Precondition: __k[i] is either 0 or -1 for all i.
   */
  template <__vec_builtin _UV, __vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr _UV
    __vec_mask_cast(_TV __k)
    {
      static_assert(signed_integral<__vec_value_type<_UV>>);
      static_assert(signed_integral<__vec_value_type<_TV>>);
      // TODO: __builtin_convertvector cannot be optimal because it doesn't consider input and
      // output can only be 0 or -1.
      return __builtin_convertvector(__k, _UV);
    }

  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr _TV
    __vec_xor(_TV __a, _TV __b)
    {
      using _Tp = __vec_value_type<_TV>;
      if constexpr (is_floating_point_v<_Tp>)
	{
	  using _UV = __vec_builtin_type<__integer_from<sizeof(_Tp)>, __width_of<_TV>>;
	  return __builtin_bit_cast(
		   _TV, __builtin_bit_cast(_UV, __a) ^ __builtin_bit_cast(_UV, __b));
	}
      else
	return __a ^ __b;
    }

  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr _TV
    __vec_or(_TV __a, _TV __b)
    {
      using _Tp = __vec_value_type<_TV>;
      if constexpr (is_floating_point_v<_Tp>)
	{
	  using _UV = __vec_builtin_type<__integer_from<sizeof(_Tp)>, __width_of<_TV>>;
	  return __builtin_bit_cast(
		   _TV, __builtin_bit_cast(_UV, __a) | __builtin_bit_cast(_UV, __b));
	}
      else
	return __a | __b;
    }

  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr _TV
    __vec_and(_TV __a, _TV __b)
    {
      using _Tp = __vec_value_type<_TV>;
      if constexpr (is_floating_point_v<_Tp>)
	{
	  using _UV = __vec_builtin_type<__integer_from<sizeof(_Tp)>, __width_of<_TV>>;
	  return __builtin_bit_cast(
		   _TV, __builtin_bit_cast(_UV, __a) & __builtin_bit_cast(_UV, __b));
	}
      else
	return __a & __b;
    }

  /** @internal
   * Returns the bit-wise and of not @p __a and @p __b.
   *
   * Use __vec_and(__vec_not(__a), __b) unless an andnot instruction is necessary for optimization.
   *
   * @see __vec_andnot in simd_x86.h
   */
  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr _TV
    __vec_andnot(_TV __a, _TV __b)
    {
      using _Tp = __vec_value_type<_TV>;
      using _UV = __vec_builtin_type<__integer_from<sizeof(_Tp)>, __width_of<_TV>>;
      return __builtin_bit_cast(
	       _TV, ~__builtin_bit_cast(_UV, __a) & __builtin_bit_cast(_UV, __b));
    }

  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    constexpr _TV
    __vec_not(_TV __a)
    {
      using _Tp = __vec_value_type<_TV>;
      using _UV = __vec_builtin_type_bytes<__integer_from<sizeof(_Tp)>, sizeof(_TV)>;
      if constexpr (is_floating_point_v<__vec_value_type<_TV>>)
	return __builtin_bit_cast(_TV, ~__builtin_bit_cast(_UV, __a));
      else
	return ~__a;
    }

  /**
   * An object of given type where only the sign bits are 1.
   */
  template <__vec_builtin _V>
    requires std::floating_point<__vec_value_type<_V>>
    constexpr _V _S_signmask = __vec_xor(_V() + 1, _V() - 1);

  template <__vec_builtin _TV, int _Np = __width_of<_TV>,
	    typename = make_integer_sequence<int, _Np>>
    struct _VecOps;

  template <__vec_builtin _TV, int _Np, int... _Is>
    struct _VecOps<_TV, _Np, integer_sequence<int, _Is...>>
    {
      static_assert(_Np <= __width_of<_TV>);

      using _Tp = __vec_value_type<_TV>;

      using _HV = __half_vec_builtin_t<__conditional_t<_Np >= 2, _TV, __double_vec_builtin_t<_TV>>>;

      [[__gnu__::__always_inline__]]
      static constexpr _TV
      _S_broadcast_to_even(_Tp __init)
      { return _TV {((_Is & 1) == 0 ? __init : _Tp())...}; }

      [[__gnu__::__always_inline__]]
      static constexpr _TV
      _S_broadcast_to_odd(_Tp __init)
      { return _TV {((_Is & 1) == 1 ? __init : _Tp())...}; }

      [[__gnu__::__always_inline__]]
      static constexpr bool
      _S_all_of(_TV __k) noexcept
      { return (... && (__k[_Is] != 0)); }

      [[__gnu__::__always_inline__]]
      static constexpr bool
      _S_any_of(_TV __k) noexcept
      { return (... || (__k[_Is] != 0)); }

      [[__gnu__::__always_inline__]]
      static constexpr bool
      _S_none_of(_TV __k) noexcept
      { return (... && (__k[_Is] == 0)); }

      template <typename _Offset = integral_constant<int, 0>>
      [[__gnu__::__always_inline__]]
      static constexpr _TV
      _S_extract(__vec_builtin auto __x, _Offset = {})
      {
	static_assert(is_same_v<__vec_value_type<_TV>, __vec_value_type<decltype(__x)>>);
	return __builtin_shufflevector(__x, decltype(__x)(), (_Is + _Offset::value)...);
      }

      // swap neighboring elements
      [[__gnu__::__always_inline__]]
      static constexpr _TV
      _S_swap_neighbors(_TV __x)
      { return __builtin_shufflevector(__x, __x, (_Is ^ 1)...); }

      // duplicate even indexed elements, dropping the odd ones
      [[__gnu__::__always_inline__]]
      static constexpr _TV
      _S_dup_even(_TV __x)
      { return __builtin_shufflevector(__x, __x, (_Is & ~1)...); }

      // duplicate odd indexed elements, dropping the even ones
      [[__gnu__::__always_inline__]]
      static constexpr _TV
      _S_dup_odd(_TV __x)
      { return __builtin_shufflevector(__x, __x, (_Is | 1)...); }

      [[__gnu__::__always_inline__]]
      static constexpr void
      _S_overwrite_even_elements(_TV& __x, _HV __y) requires (_Np > 1)
      {
	constexpr __simd_size_type __n = __width_of<_TV>;
	__x = __builtin_shufflevector(__x,
#ifdef _GLIBCXX_CLANG
				      __vec_concat(__y, __y),
#else
				      __y,
#endif
				      ((_Is & 1) == 0 ? __n + _Is / 2 : _Is)...);
      }

      [[__gnu__::__always_inline__]]
      static constexpr void
      _S_overwrite_even_elements(_TV& __xl, _TV& __xh, _TV __y)
      {
	constexpr __simd_size_type __nl = __width_of<_TV>;
	constexpr __simd_size_type __nh = __nl * 3 / 2;
	__xl = __builtin_shufflevector(__xl, __y, ((_Is & 1) == 0 ? __nl + _Is / 2 : _Is)...);
	__xh = __builtin_shufflevector(__xh, __y, ((_Is & 1) == 0 ? __nh + _Is / 2 : _Is)...);
      }

      [[__gnu__::__always_inline__]]
      static constexpr void
      _S_overwrite_odd_elements(_TV& __x, _HV __y) requires (_Np > 1)
      {
	constexpr __simd_size_type __n = __width_of<_TV>;
	__x = __builtin_shufflevector(__x,
#ifdef _GLIBCXX_CLANG
				      __vec_concat(__y, __y),
#else
				      __y,
#endif
				      ((_Is & 1) == 1 ? __n + _Is / 2 : _Is)...);
      }

      [[__gnu__::__always_inline__]]
      static constexpr void
      _S_overwrite_odd_elements(_TV& __xl, _TV& __xh, _TV __y)
      {
	constexpr __simd_size_type __nl = __width_of<_TV>;
	constexpr __simd_size_type __nh = __nl * 3 / 2;
	__xl = __builtin_shufflevector(__xl, __y, ((_Is & 1) == 1 ? __nl + _Is / 2 : _Is)...);
	__xh = __builtin_shufflevector(__xh, __y, ((_Is & 1) == 1 ? __nh + _Is / 2 : _Is)...);
      }

      // true if all elements are know to be equal to __ref at compile time
      [[__gnu__::__always_inline__]]
      static constexpr bool
      _S_is_const_known_equal_to(_TV __x, _Tp __ref)
      { return (__is_const_known_equal_to(__x[_Is], __ref) && ...); }

    };
} // namespace simd
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#pragma GCC diagnostic pop
#endif // C++26
#endif // _GLIBCXX_VEC_OPS_H
