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

#ifndef _GLIBCXX_SIMD_X86_H
#define _GLIBCXX_SIMD_X86_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#if __cplusplus >= 202400L

#include "vec_ops.h"

#if !_GLIBCXX_X86
#error "wrong include for this target"
#endif

#pragma GCC push_options
// ensure GCC knows about the __builtin_ia32_* calls
#pragma GCC target("avx2,bmi,bmi2,avx512vl,avx512bw,avx512dq,avx10.2")
#pragma GCC pop_options

// psabi warnings are bogus because the ABI of the internal types never leaks into user code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi"

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace simd
{
  static constexpr size_t __x86_max_general_register_size
#ifdef __x86_64__
    = 8;
#else
    = 4;
#endif

  /** @internal
   * Return a bit-mask for the given vector-mask.
   *
   * Caveats:
   * 1. The bit-mask of 2-Byte vector-masks has duplicated entries (because of missing instruction)
   * 2. The return type internally is 'int', but that fails on conversion to uint64 if the MSB of a
   * YMM 1/2-Byte vector-mask is set (sign extension). Therefore these helper functions return
   * unsigned instead.
   * 3. ZMM inputs are not supported.
   */
  [[__gnu__::__always_inline__]]
  inline unsigned
  __x86_movmsk(__vec_builtin_type_bytes<__integer_from<8>, 16> __x)
  { return __builtin_ia32_movmskpd(__vec_bit_cast<double>(__x)); }

  [[__gnu__::__always_inline__]]
  inline unsigned
  __x86_movmsk(__vec_builtin_type_bytes<__integer_from<8>, 32> __x)
  { return __builtin_ia32_movmskpd256(__vec_bit_cast<double>(__x)); }

  [[__gnu__::__always_inline__]]
  inline unsigned
  __x86_movmsk(__vec_builtin_type_bytes<__integer_from<4>, 16> __x)
  { return __builtin_ia32_movmskps(__vec_bit_cast<float>(__x)); }

  template <_ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline _Bitmask<8>
    __x86_movmsk(__vec_builtin_type_bytes<__integer_from<4>, 8> __x)
    {
#if __has_builtin(__builtin_ia32_pext_di)
      if constexpr (_Traits._M_have_bmi2())
	return _Bitmask<8>(__builtin_ia32_pext_di(
			     __builtin_bit_cast(unsigned long long, __x),
			     0x80000000'80000000ULL));
#endif
      return _Bitmask<8>(__x86_movmsk(__vec_zero_pad_to_16(__x)));
    }

  [[__gnu__::__always_inline__]]
  inline unsigned
  __x86_movmsk(__vec_builtin_type_bytes<__integer_from<4>, 32> __x)
  { return __builtin_ia32_movmskps256(__vec_bit_cast<float>(__x)); }

  template <__vec_builtin _TV, auto _Traits = _ArchTraits()>
    requires (sizeof(__vec_value_type<_TV>) <= 2)
    [[__gnu__::__always_inline__]]
    inline unsigned
    __x86_movmsk(_TV __x)
    {
      static_assert(__width_of<_TV> > 1);
      if constexpr (sizeof(__x) == 32)
	return __builtin_ia32_pmovmskb256(__vec_bit_cast<char>(__x));
      else if constexpr (sizeof(__x) == 16)
	return __builtin_ia32_pmovmskb128(__vec_bit_cast<char>(__x));
      else if constexpr (sizeof(__x) == 8)
	{
#if __has_builtin(__builtin_ia32_pext_di)
	  if constexpr (_Traits._M_have_bmi2())
	    return __builtin_ia32_pext_di(__builtin_bit_cast(unsigned long long, __x),
					  0x8080'8080'8080'8080ULL);
#endif
	  return __x86_movmsk(__vec_zero_pad_to_16(__x));
	}
      else if constexpr (sizeof(__x) == 4)
	{
#if __has_builtin(__builtin_ia32_pext_si)
	  if constexpr (_Traits._M_have_bmi2())
	    return __builtin_ia32_pext_si(__builtin_bit_cast(unsigned int, __x), 0x80808080u);
#endif
	  return __x86_movmsk(__vec_zero_pad_to_16(__x));
	}
      else if constexpr (sizeof(__x) == 2)
	{
	  auto __bits = __builtin_bit_cast(unsigned short, __x);
#if __has_builtin(__builtin_ia32_pext_si)
	  if constexpr (_Traits._M_have_bmi2())
	    return __builtin_ia32_pext_si(__bits, 0x00008080u);
#endif
	  return ((__bits >> 7) & 1) | ((__bits & 0x8000) >> 14);
	}
      else
	static_assert(false);
    }

  template <__vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline bool
    __x86_vec_is_zero(_TV __a)
    {
      using _Tp = __vec_value_type<_TV>;
      static_assert(is_integral_v<_Tp>);
      if constexpr (sizeof(_TV) <= __x86_max_general_register_size)
	return __builtin_bit_cast(__integer_from<sizeof(_TV)>, __a) == 0;
      else if constexpr (_Traits._M_have_avx())
	{
	  if constexpr (sizeof(_TV) == 32)
	    return __builtin_ia32_ptestz256(__vec_bit_cast<long long>(__a),
					    __vec_bit_cast<long long>(__a));
	  else if constexpr (sizeof(_TV) == 16)
	    return __builtin_ia32_ptestz128(__vec_bit_cast<long long>(__a),
					    __vec_bit_cast<long long>(__a));
	  else if constexpr (sizeof(_TV) < 16)
	    return __x86_vec_is_zero(__vec_zero_pad_to_16(__a));
	  else
	    static_assert(false);
	}
      else if constexpr (_Traits._M_have_sse4_1())
	{
	  if constexpr (sizeof(_TV) == 16)
	    return __builtin_ia32_ptestz128(__vec_bit_cast<long long>(__a),
					    __vec_bit_cast<long long>(__a));
	  else if constexpr (sizeof(_TV) < 16)
	    return __x86_vec_is_zero(__vec_zero_pad_to_16(__a));
	  else
	    static_assert(false);
	}
      else
	return __x86_movmsk(__a) == 0;
    }

  template <__vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline int
    __x86_vec_testz(_TV __a, _TV __b)
    {
      static_assert(sizeof(_TV) == 16 || sizeof(_TV) == 32);
      static_assert(_Traits._M_have_sse4_1());
      if constexpr (sizeof(_TV) == 32)
	return __builtin_ia32_ptestz256(__vec_bit_cast<long long>(__a),
					__vec_bit_cast<long long>(__b));
      else
	return __builtin_ia32_ptestz128(__vec_bit_cast<long long>(__a),
					__vec_bit_cast<long long>(__b));
    }

  template <__vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline int
    __x86_vec_testc(_TV __a, _TV __b)
    {
      static_assert(sizeof(_TV) == 16 || sizeof(_TV) == 32);
      static_assert(_Traits._M_have_sse4_1());
      if constexpr (sizeof(_TV) == 32)
	return __builtin_ia32_ptestc256(__vec_bit_cast<long long>(__a),
					__vec_bit_cast<long long>(__b));
      else
	return __builtin_ia32_ptestc128(__vec_bit_cast<long long>(__a),
					__vec_bit_cast<long long>(__b));
    }

  template <int _Np, __vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline bool
    __x86_vecmask_all(_TV __k)
    {
      using _Tp = __vec_value_type<_TV>;
      static_assert(is_integral_v<_Tp> && is_signed_v<_Tp>);
      constexpr int __width = __width_of<_TV>;
      static_assert(sizeof(__k) <= 32);
      if constexpr (_Np == __width)
	{
	  if constexpr (sizeof(__k) <= __x86_max_general_register_size)
	    {
	      using _Ip = __integer_from<sizeof(__k)>;
	      return __builtin_bit_cast(_Ip, __k) == ~_Ip();
	    }
	  else if constexpr (!_Traits._M_have_sse4_1())
	    {
	      constexpr unsigned __valid_bits = (1u << (sizeof(_Tp) == 2 ? _Np * 2 : _Np)) - 1;
	      return __x86_movmsk(__k) == __valid_bits;
	    }
	  else if constexpr (sizeof(__k) < 16)
	    return __x86_vecmask_all<_Np>(__vec_zero_pad_to_16(__k));
	  else
	    return 0 != __x86_vec_testc(__k, ~_TV());
	}
      else if constexpr (sizeof(__k) <= __x86_max_general_register_size)
	{
	  using _Ip = __integer_from<sizeof(__k)>;
	  constexpr _Ip __valid_bits = (_Ip(1) << (_Np * sizeof(_Tp) * __CHAR_BIT__)) - 1;
	  return (__builtin_bit_cast(_Ip, __k) & __valid_bits) == __valid_bits;
	}
      else if constexpr (!_Traits._M_have_sse4_1())
	{
	  constexpr unsigned __valid_bits = (1u << (sizeof(_Tp) == 2 ? _Np * 2 : _Np)) - 1;
	  return (__x86_movmsk(__k) & __valid_bits) == __valid_bits;
	}
      else if constexpr (sizeof(__k) < 16)
	return __x86_vecmask_all<_Np>(__vec_zero_pad_to_16(__k));
      else
	return 0 != __x86_vec_testc(__k, _S_vec_implicit_mask<_Np, _TV>);
    }

  template <int _Np, __vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline bool
    __x86_vecmask_any(_TV __k)
    {
      using _Tp = __vec_value_type<_TV>;
      static_assert(is_integral_v<_Tp> && is_signed_v<_Tp>);
      constexpr int __width = __width_of<_TV>;
      static_assert(sizeof(__k) <= 32);
      if constexpr (_Np == __width)
	return !__x86_vec_is_zero(__k);
      else if constexpr (sizeof(__k) <= __x86_max_general_register_size)
	{
	  using _Ip = __integer_from<sizeof(__k)>;
	  constexpr _Ip __valid_bits = (_Ip(1) << (_Np * sizeof(_Tp) * __CHAR_BIT__)) - 1;
	  return (__builtin_bit_cast(_Ip, __k) & __valid_bits) != _Ip();
	}
      else if constexpr (!_Traits._M_have_sse4_1())
	{
	  constexpr unsigned __valid_bits = (1u << (sizeof(_Tp) == 2 ? _Np * 2 : _Np)) - 1;
	  return (__x86_movmsk(__k) & __valid_bits) != 0;
	}
      else if constexpr (sizeof(__k) < 16)
	return __x86_vecmask_any<_Np>(__vec_zero_pad_to_16(__k));
      else
	return 0 == __x86_vec_testz(__k, _S_vec_implicit_mask<_Np, _TV>);
    }

  template <int _Np, __vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline bool
    __x86_vecmask_none(_TV __k)
    {
      using _Tp = __vec_value_type<_TV>;
      static_assert(is_integral_v<_Tp> && is_signed_v<_Tp>);
      constexpr int __width = __width_of<_TV>;
      static_assert(sizeof(__k) <= 32);
      if constexpr (_Np == __width)
	return __x86_vec_is_zero(__k);
      else if constexpr (sizeof(__k) <= __x86_max_general_register_size)
	{
	  using _Ip = __integer_from<sizeof(__k)>;
	  constexpr _Ip __valid_bits = (_Ip(1) << (_Np * sizeof(_Tp) * __CHAR_BIT__)) - 1;
	  return (__builtin_bit_cast(_Ip, __k) & __valid_bits) == _Ip();
	}
      else if constexpr (!_Traits._M_have_sse4_1())
	{
	  constexpr unsigned __valid_bits = (1u << (sizeof(_Tp) == 2 ? _Np * 2 : _Np)) - 1;
	  return (__x86_movmsk(__k) & __valid_bits) == 0;
	}
      else if constexpr (sizeof(__k) < 16)
	return __x86_vecmask_none<_Np>(__vec_zero_pad_to_16(__k));
      else
	return 0 != __x86_vec_testz(__k, _S_vec_implicit_mask<_Np, _TV>);
    }

  enum class _X86Cmp
  {
    _Eq = 0,
    _Lt = 1,
    _Le = 2,
    _Unord = 3,
    _Neq = 4,
    _Nlt = 5,
    _Nle = 6,
  };

  template <_X86Cmp _Cmp, __vec_builtin _TV, _ArchTraits _Traits = {}>
    requires is_floating_point_v<__vec_value_type<_TV>>
    [[__gnu__::__always_inline__]]
    inline auto
    __x86_bitmask_cmp(_TV __x, _TV __y)
    {
      constexpr int __c = int(_Cmp);
      using _Tp = __vec_value_type<_TV>;
      if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 8)
	return __builtin_ia32_cmppd512_mask(__x, __y, __c, -1, 4);
      else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 4)
	return __builtin_ia32_cmpps512_mask(__x, __y, __c, -1, 4);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 8)
	return __builtin_ia32_cmppd256_mask(__x, __y, __c, -1);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 4)
	return __builtin_ia32_cmpps256_mask(__x, __y, __c, -1);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 8)
	return __builtin_ia32_cmppd128_mask(__x, __y, __c, -1);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 4)
	return __builtin_ia32_cmpps128_mask(__x, __y, __c, -1);
      else if constexpr (is_same_v<_Tp, _Float16>)
	{
	  if constexpr (sizeof(_TV) == 64 && _Traits._M_have_avx512fp16())
	    return __builtin_ia32_cmpph512_mask(__x, __y, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && _Traits._M_have_avx512fp16())
	    return __builtin_ia32_cmpph256_mask(__x, __y, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && _Traits._M_have_avx512fp16())
	    return __builtin_ia32_cmpph128_mask(__x, __y, __c, -1);
	  else if constexpr (sizeof(_TV) < 16 && _Traits._M_have_avx512fp16())
	    return __x86_bitmask_cmp<_Cmp>(__vec_zero_pad_to_16(__x), __vec_zero_pad_to_16(__y));
	  else
	    {
	      // without AVX512_FP16, float16_t size needs to match float32_t size
	      // (cf. __native_abi())
	      static_assert(sizeof(_TV) <= 32);
	      return __x86_bitmask_cmp<_Cmp>(__vec_cast<float>(__x), __vec_cast<float>(__y));
	    }
	}
      else if constexpr (sizeof(_TV) < 16)
	return __x86_bitmask_cmp<_Cmp>(__vec_zero_pad_to_16(__x), __vec_zero_pad_to_16(__y));
      else
	static_assert(false);
    }

  template <typename _Tp>
    using __x86_intrin_int
      = decltype([] {
	  if constexpr (sizeof(_Tp) == 1)
	    return char();
	  else
	    return __integer_from<sizeof(_Tp)>();
	}());

  template <typename _Tp>
    using __x86_intrin_type
      = decltype([] {
	  if constexpr (is_integral_v<_Tp> || sizeof(_Tp) <= 2)
	    return __x86_intrin_int<_Tp>();
	  else
	    return __canonical_vec_type_t<_Tp>();
	}());

  template <typename _Tp>
    using __x86_intel_intrin_value_type
      = decltype([] {
	  if constexpr (is_integral_v<_Tp>)
	    return 0ll;
	  else if constexpr (sizeof(_Tp) == 8)
	    return 0.;
	  else if constexpr (sizeof(_Tp) == 4)
	    return 0.f;
	  else if constexpr (sizeof(_Tp) == 2)
	    return 0.f16;
	}());

#if !_GLIBCXX_CLANG
  // overload __vec_andnot from simd_detail.h
  template <__vec_builtin _TV>
    requires (sizeof(_TV) >= 16)
    [[__gnu__::__always_inline__]]
    constexpr _TV
    __vec_andnot(_TV __a, _TV __b)
    {
      constexpr _TargetTraits _Traits = {};
      using _Tp = __vec_value_type<_TV>;
      using _UV = __vec_builtin_type<_UInt<sizeof(_Tp)>, __width_of<_TV>>;
      if (__builtin_is_constant_evaluated()
	    || (__builtin_constant_p(__a) && __builtin_constant_p(__b)))
	return reinterpret_cast<_TV>(~reinterpret_cast<_UV>(__a) & reinterpret_cast<_UV>(__b));
      else if constexpr (is_same_v<_Tp, _Float16>)
	return reinterpret_cast<_TV>(__vec_andnot(__vec_bit_cast<float>(__a),
						  __vec_bit_cast<float>(__b)));
      else if constexpr (sizeof(_TV) == 16 && is_same_v<_Tp, float>)
	return __builtin_ia32_andnps(__a, __b);
      else if constexpr (sizeof(_TV) == 16 && is_same_v<_Tp, double>)
	return __builtin_ia32_andnpd(__a, __b);
      else if constexpr (sizeof(_TV) == 32 && is_same_v<_Tp, float>)
	return __builtin_ia32_andnps256(__a, __b);
      else if constexpr (sizeof(_TV) == 32 && is_same_v<_Tp, double>)
	return __builtin_ia32_andnpd256(__a, __b);
      else if constexpr (sizeof(_TV) == 64 && is_same_v<_Tp, float> && _Traits._M_have_avx512dq())
	return __builtin_ia32_andnps512_mask(__a, __b, _TV{}, -1);
      else if constexpr (sizeof(_TV) == 64 && is_same_v<_Tp, double> && _Traits._M_have_avx512dq())
	return __builtin_ia32_andnpd512_mask(__a, __b, _TV{}, -1);
      else
	{
	  auto __all = __vec_bit_cast<long long>(__a);
	  auto __bll = __vec_bit_cast<long long>(__b);
	  if constexpr (sizeof(_TV) == 16 && is_integral_v<_Tp>)
	    return reinterpret_cast<_TV>(__builtin_ia32_pandn128(__all, __bll));
	  else if constexpr (sizeof(_TV) == 32 && is_integral_v<_Tp> && _Traits._M_have_avx2())
	    return reinterpret_cast<_TV>(__builtin_ia32_andnotsi256(__all, __bll));
	  else if constexpr (sizeof(_TV) == 32 && is_integral_v<_Tp>)
	    return reinterpret_cast<_TV>(__builtin_ia32_andnpd256(__vec_bit_cast<double>(__a),
								  __vec_bit_cast<double>(__b)));
	  else if constexpr (sizeof(_TV) == 64)
	    {
	      auto __ai = __vec_bit_cast<int>(__a);
	      auto __bi = __vec_bit_cast<int>(__b);
	      return reinterpret_cast<_TV>(
		       __builtin_ia32_pandnd512_mask(__ai, __bi, decltype(__ai)(), -1));
	    }
	}
    }
#endif // not Clang

  template <_X86Cmp _Cmp, __vec_builtin _TV, _ArchTraits _Traits = {}>
    requires is_integral_v<__vec_value_type<_TV>>
    [[__gnu__::__always_inline__]]
    inline auto
    __x86_bitmask_cmp(_TV __x, _TV __y)
    {
      constexpr int __c = int(_Cmp);
      using _Tp = __vec_value_type<_TV>;
      if constexpr (sizeof(_TV) < 16)
	return __x86_bitmask_cmp<_Cmp>(__vec_zero_pad_to_16(__x), __vec_zero_pad_to_16(__y));
      else if constexpr (is_signed_v<_Tp>)
	{
	  const auto __xi = __vec_bit_cast<__x86_intrin_int<_Tp>>(__x);
	  const auto __yi = __vec_bit_cast<__x86_intrin_int<_Tp>>(__y);
	  if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 8)
	    return __builtin_ia32_cmpq512_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 4)
	    return __builtin_ia32_cmpd512_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 2)
	    return __builtin_ia32_cmpw512_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 1)
	    return __builtin_ia32_cmpb512_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 8)
	    return __builtin_ia32_cmpq256_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 4)
	    return __builtin_ia32_cmpd256_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 2)
	    return __builtin_ia32_cmpw256_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 1)
	    return __builtin_ia32_cmpb256_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 8)
	    return __builtin_ia32_cmpq128_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 4)
	    return __builtin_ia32_cmpd128_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 2)
	    return __builtin_ia32_cmpw128_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 1)
	    return __builtin_ia32_cmpb128_mask(__xi, __yi, __c, -1);
	  else
	    static_assert(false);
	}
      else
	{
	  const auto __xi = __vec_bit_cast<__x86_intrin_int<_Tp>>(__x);
	  const auto __yi = __vec_bit_cast<__x86_intrin_int<_Tp>>(__y);
	  if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 8)
	    return __builtin_ia32_ucmpq512_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 4)
	    return __builtin_ia32_ucmpd512_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 2)
	    return __builtin_ia32_ucmpw512_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 1)
	    return __builtin_ia32_ucmpb512_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 8)
	    return __builtin_ia32_ucmpq256_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 4)
	    return __builtin_ia32_ucmpd256_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 2)
	    return __builtin_ia32_ucmpw256_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 1)
	    return __builtin_ia32_ucmpb256_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 8)
	    return __builtin_ia32_ucmpq128_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 4)
	    return __builtin_ia32_ucmpd128_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 2)
	    return __builtin_ia32_ucmpw128_mask(__xi, __yi, __c, -1);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 1)
	    return __builtin_ia32_ucmpb128_mask(__xi, __yi, __c, -1);
	  else
	    static_assert(false);
	}
    }

  template <__vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline auto
    __x86_bitmask_isinf(_TV __x)
    {
      static_assert(_Traits._M_have_avx512dq());
      using _Tp = __vec_value_type<_TV>;
      static_assert(is_floating_point_v<_Tp>);
      if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 8)
	return __builtin_ia32_fpclasspd512_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 8)
	return __builtin_ia32_fpclasspd256_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 8)
	return __builtin_ia32_fpclasspd128_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 4)
	return __builtin_ia32_fpclassps512_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 4)
	return __builtin_ia32_fpclassps256_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 4)
	return __builtin_ia32_fpclassps128_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 2 && _Traits._M_have_avx512fp16())
	return __builtin_ia32_fpclassph512_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 2 && _Traits._M_have_avx512fp16())
	return __builtin_ia32_fpclassph256_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 2 && _Traits._M_have_avx512fp16())
	return __builtin_ia32_fpclassph128_mask(__x, 0x18, -1);
      else if constexpr (sizeof(_Tp) == 2 && !_Traits._M_have_avx512fp16())
	return __x86_bitmask_isinf(__vec_cast<float>(__x));
      else if constexpr (sizeof(_TV) < 16)
	return __x86_bitmask_isinf(__vec_zero_pad_to_16(__x));
      else
	static_assert(false);
    }

  template <__vec_builtin _KV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline _KV
    __x86_bit_to_vecmask(std::integral auto __bits)
    {
      using _Kp = __vec_value_type<_KV>;
      static_assert((sizeof(__bits) * __CHAR_BIT__ == __width_of<_KV>)
		      || (sizeof(__bits) == 1 && __CHAR_BIT__ > __width_of<_KV>));

      if constexpr (sizeof(_Kp) == 1 && sizeof(_KV) == 64)
	return __builtin_ia32_cvtmask2b512(__bits);
      else if constexpr (sizeof(_Kp) == 1 && sizeof(_KV) == 32)
	return __builtin_ia32_cvtmask2b256(__bits);
      else if constexpr (sizeof(_Kp) == 1 && sizeof(_KV) == 16)
	return __builtin_ia32_cvtmask2b128(__bits);
      else if constexpr (sizeof(_Kp) == 1 && sizeof(_KV) <= 8)
	return _VecOps<_KV>::_S_extract(__builtin_ia32_cvtmask2b128(__bits));

      else if constexpr (sizeof(_Kp) == 2 && sizeof(_KV) == 64)
	return __builtin_ia32_cvtmask2w512(__bits);
      else if constexpr (sizeof(_Kp) == 2 && sizeof(_KV) == 32)
	return __builtin_ia32_cvtmask2w256(__bits);
      else if constexpr (sizeof(_Kp) == 2 && sizeof(_KV) == 16)
	return __builtin_ia32_cvtmask2w128(__bits);
      else if constexpr (sizeof(_Kp) == 2 && sizeof(_KV) <= 8)
	return _VecOps<_KV>::_S_extract(__builtin_ia32_cvtmask2w128(__bits));

      else if constexpr (sizeof(_Kp) == 4 && sizeof(_KV) == 64)
	return __builtin_ia32_cvtmask2d512(__bits);
      else if constexpr (sizeof(_Kp) == 4 && sizeof(_KV) == 32)
	return __builtin_ia32_cvtmask2d256(__bits);
      else if constexpr (sizeof(_Kp) == 4 && sizeof(_KV) <= 16)
	return _VecOps<_KV>::_S_extract(__builtin_ia32_cvtmask2d128(__bits));

      else if constexpr (sizeof(_Kp) == 8 && sizeof(_KV) == 64)
	return __builtin_ia32_cvtmask2q512(__bits);
      else if constexpr (sizeof(_Kp) == 8 && sizeof(_KV) == 32)
	return __builtin_ia32_cvtmask2q256(__bits);
      else if constexpr (sizeof(_Kp) == 8 && sizeof(_KV) == 16)
	return __builtin_ia32_cvtmask2q128(__bits);

      else
	static_assert(false);
    }

  template <unsigned_integral _Kp, __vec_builtin _TV, _ArchTraits _Traits = {}>
    requires is_integral_v<__vec_value_type<_TV>>
    [[__gnu__::__always_inline__]]
    constexpr inline _TV
    __x86_bitmask_blend(_Kp __k, _TV __t, _TV __f)
    {
      using _Tp = __vec_value_type<_TV>;
      using _Ip = __x86_intrin_int<_Tp>;
      if constexpr (!is_same_v<_Ip, _Tp>)
	return reinterpret_cast<_TV>(__x86_bitmask_blend(__k, __vec_bit_cast<_Ip>(__t),
							 __vec_bit_cast<_Ip>(__f)));
      else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 8)
	return __builtin_ia32_blendmq_512_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 4)
	return __builtin_ia32_blendmd_512_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 2)
	return __builtin_ia32_blendmw_512_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 1)
	return __builtin_ia32_blendmb_512_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 8)
	return __builtin_ia32_blendmq_256_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 4)
	return __builtin_ia32_blendmd_256_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 2)
	return __builtin_ia32_blendmw_256_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 1)
	return __builtin_ia32_blendmb_256_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 8)
	return __builtin_ia32_blendmq_128_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 4)
	return __builtin_ia32_blendmd_128_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 2)
	return __builtin_ia32_blendmw_128_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 1)
	return __builtin_ia32_blendmb_128_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) < 16)
	return _VecOps<_TV>::_S_extract(__x86_bitmask_blend(__k, __vec_zero_pad_to_16(__t),
							    __vec_zero_pad_to_16(__f)));
      else
	static_assert(false);
    }

  template <unsigned_integral _Kp, __vec_builtin _TV, _ArchTraits _Traits = {}>
    requires is_floating_point_v<__vec_value_type<_TV>>
    [[__gnu__::__always_inline__]]
    constexpr inline _TV
    __x86_bitmask_blend(_Kp __k, _TV __t, _TV __f)
    {
      using _Tp = __vec_value_type<_TV>;
      if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 8)
	return __builtin_ia32_blendmpd_512_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 64 && sizeof(_Tp) == 4)
	return __builtin_ia32_blendmps_512_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 8)
	return __builtin_ia32_blendmpd_256_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 4)
	return __builtin_ia32_blendmps_256_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 8)
	return __builtin_ia32_blendmpd_128_mask (__f, __t, __k);
      else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 4)
	return __builtin_ia32_blendmps_128_mask (__f, __t, __k);
      else if constexpr (is_same_v<_Tp, _Float16>)
	{
	  using _Up = __integer_from<sizeof(_Tp)>;
	  return __vec_bit_cast<_Float16>(__x86_bitmask_blend(__k, __vec_bit_cast<_Up>(__t),
							      __vec_bit_cast<_Up>(__f)));
	}
      else if constexpr (sizeof(_TV) < 16)
	return _VecOps<_TV>::_S_extract(__x86_bitmask_blend(__k, __vec_zero_pad_to_16(__t),
							    __vec_zero_pad_to_16(__f)));
      else
	static_assert(false);
    }

  template <int _OutputBits = 4, _ArchTraits _Traits = {}>
    constexpr _Bitmask<1>
    __bit_extract_even(_UInt<1> __x)
    {
      static_assert(_OutputBits <= 4);
      constexpr _UInt<1> __mask = 0x55u >> ((4 - _OutputBits) * 2);
#if __has_builtin(__builtin_ia32_pext_si)
      if constexpr (_Traits._M_have_bmi2())
	return __builtin_ia32_pext_si(__x, __mask);
#endif
      __x &= __mask;
      __x |= __x >> 1;
      __x &= 0x33u;
      __x |= __x >> 2;
      __x &= 0x0Fu;
      return __x;
    }

  template <int _OutputBits = 8, _ArchTraits _Traits = {}>
    constexpr _Bitmask<1>
    __bit_extract_even(_UInt<2> __x)
    {
      if constexpr (_OutputBits <= 4)
	return __bit_extract_even<_OutputBits>(_UInt<1>(__x));
      else
	{
	  static_assert(_OutputBits <= 8);
	  constexpr _UInt<2> __mask = 0x5555u >> ((8 - _OutputBits) * 2);
#if __has_builtin(__builtin_ia32_pext_si)
	  if constexpr (_Traits._M_have_bmi2())
	    return __builtin_ia32_pext_si(__x, __mask);
#endif
	  __x &= __mask;
	  __x |= __x >> 1;
	  __x &= 0x3333u;
	  __x |= __x >> 2;
	  __x &= 0x0F0Fu;
	  __x |= __x >> 4;
	  return __x;
	}
    }

  template <int _OutputBits = 16, _ArchTraits _Traits = {}>
    constexpr _Bitmask<_OutputBits>
    __bit_extract_even(_UInt<4> __x)
    {
      if constexpr (_OutputBits <= 4)
	return __bit_extract_even<_OutputBits>(_UInt<1>(__x));
      else if constexpr (_OutputBits <= 8)
	return __bit_extract_even<_OutputBits>(_UInt<2>(__x));
      else
	{
	  static_assert(_OutputBits <= 16);
	  constexpr _UInt<4> __mask = 0x5555'5555u >> ((16 - _OutputBits) * 2);
#if __has_builtin(__builtin_ia32_pext_si)
	  if constexpr (_Traits._M_have_bmi2())
	    return __builtin_ia32_pext_si(__x, __mask);
#endif
	  __x &= __mask;
	  __x |= __x >> 1;
	  __x &= 0x3333'3333u;
	  __x |= __x >> 2;
	  __x &= 0x0F0F'0F0Fu;
	  __x |= __x >> 4;
	  __x &= 0x00FF'00FFu;
	  __x |= __x >> 8;
	  return __x;
	}
    }

  template <int _OutputBits = 32, _ArchTraits _Traits = {}>
    constexpr _Bitmask<_OutputBits>
    __bit_extract_even(_UInt<8> __x)
    {
      if constexpr (_OutputBits <= 4)
	return __bit_extract_even<_OutputBits>(_UInt<1>(__x));
      else if constexpr (_OutputBits <= 8)
	return __bit_extract_even<_OutputBits>(_UInt<2>(__x));
      else if constexpr (_OutputBits <= 16)
	return __bit_extract_even<_OutputBits>(_UInt<4>(__x));
      else
	{
	  static_assert(_OutputBits <= 32);
	  constexpr _UInt<8> __mask = 0x5555'5555'5555'5555ull >> ((32 - _OutputBits) * 2);
#if __has_builtin(__builtin_ia32_pext_si)
	  if constexpr (_Traits._M_have_bmi2())
	    {
#if __has_builtin(__builtin_ia32_pext_di)
	      return __builtin_ia32_pext_di(__x, __mask);
#else
	      return __builtin_ia32_pext_si(__x, static_cast<unsigned>(__mask))
		       | (__builtin_ia32_pext_si(__x >> 32, __mask >> 32) << 16);
#endif
	    }
#endif
	  __x &= __mask;
	  __x |= __x >> 1;
	  __x &= 0x3333'3333'3333'3333ull;
	  __x |= __x >> 2;
	  __x &= 0x0F0F'0F0F'0F0F'0F0Full;
	  __x |= __x >> 4;
	  __x &= 0x00FF'00FF'00FF'00FFull;
	  __x |= __x >> 8;
	  __x &= 0x0000'FFFF'0000'FFFFull;
	  __x |= __x >> 16;
	  return __x;
	}
    }

  // input bits must be 0 for all bits > _InputBits
  template <int _InputBits = -1, _ArchTraits _Traits = {}>
    constexpr auto
    __duplicate_each_bit(unsigned_integral auto __x)
    {
      constexpr int __input_bits = _InputBits == -1 ? sizeof(__x) * __CHAR_BIT__ : _InputBits;
      static_assert(__input_bits >= 1);
      static_assert(sizeof(__x) * __CHAR_BIT__ >= __input_bits);
      if constexpr (__input_bits <= 8)
	{
	  constexpr _UInt<2> __mask = 0x5555u >> ((8 - __input_bits) * 2);
	  if constexpr (__input_bits == 1)
	    return _UInt<1>(__x * 3u);
#if __has_builtin(__builtin_ia32_pdep_si)
	  else if constexpr (_Traits._M_have_bmi2())
	    return _Bitmask<__input_bits * 2>(3u * __builtin_ia32_pdep_si(__x, __mask));
#endif
	  else if constexpr (__input_bits == 2) // 0000'00BA
	    return _UInt<1>(((__x + 0b0010u) & 0b0101u) * 3u); // 0B?A -> 0B0A -> BBAA
	  else if constexpr (__input_bits <= 4) // 0000'DCBA
	    {
	      __x = ((__x << 2) | __x ) & 0b0011'0011u; // 00DC'??BA -> 00DC'00BA
	      return _UInt<1>(((__x + 0b0010'0010u) & __mask) * 3u);     // -> DDCC'BBAA
	    }
	  else
	    { // HGFE'DCBA
	      _UInt<2> __y = ((__x << 4) | __x) & 0x0F0Fu; // HGFE'0000'DCBA
	      __y |= __y << 2; // 00HG'??FE'00DC'??BA
	      __y &= 0x3333u;  // 00HG'00FE'00DC'00BA
	      __y += 0x2222u;  // 0H?G'0F?E'0D?C'0B?A
	      return _UInt<2>((__y & __mask) * 3u); // HHGG'FFEE'DDCC'BBAA
	    }
	}
      else if constexpr (__input_bits <= 16)
	{
	  constexpr _UInt<4> __mask = 0x5555'5555u >> ((16 - __input_bits) * 2);
#if __has_builtin(__builtin_ia32_pdep_si)
	  if constexpr (_Traits._M_have_bmi2())
	    return 3u * __builtin_ia32_pdep_si(__x, __mask);
#endif
	  _UInt<4> __y = ((__x << 8) | __x) & 0x00FF00FFu;
	  __y |= __y << 4;
	  __y &= 0x0F0F'0F0Fu;
	  __y |= __y << 2;
	  __y &= 0x3333'3333u;
	  return ((__y + 0x2222'2222u) & __mask) * 3;
	}
      else if constexpr (__input_bits <= 32)
	{
	  constexpr _UInt<8> __mask = 0x5555'5555'5555'5555u >> ((32 - __input_bits) * 2);
#if __has_builtin(__builtin_ia32_pdep_si)
	  if constexpr (_Traits._M_have_bmi2())
	    {
#if __has_builtin(__builtin_ia32_pdep_di)
	      return 3ull * __builtin_ia32_pdep_di(__x, __mask);
#else
	      const _UInt<8> __hi = 3 * __builtin_ia32_pdep_si(__x >> 16, __mask >> 32);
	      return (3u * __builtin_ia32_pdep_si(__x, static_cast<unsigned>(__mask))) | __hi << 32;
#endif
	    }
#endif
	  _UInt<8> __y = ((__x & 0xFFFF'0000ull) << 16) | (__x & 0x0000'FFFFu);
	  __y |= __y << 8;
	  __y &= 0x00FF'00FF'00FF'00FFull;
	  __y |= __y << 4;
	  __y &= 0x0F0F'0F0F'0F0F'0F0Full;
	  __y |= __y << 2;
	  __y &= 0x3333'3333'3333'3333ull;
	  return ((__y + 0x2222'2222'2222'2222ull) & __mask) * 3;
	}
      else
	return __trivial_pair { __duplicate_each_bit(_UInt<4>(__x)),
				__duplicate_each_bit<__input_bits - 32>(
				  _Bitmask<__input_bits - 32>(__x >> 32)) };
    }

  template <int _InputBits = -1, typename _U0, typename _U1>
    constexpr auto
    __duplicate_each_bit(const __trivial_pair<_U0, _U1>& __x)
    {
      static_assert(_InputBits != -1 || is_unsigned_v<_U1>);
      constexpr int __input_bits = _InputBits == -1 ? (sizeof(_U0) + sizeof(_U1)) * __CHAR_BIT__
						    : _InputBits;
      constexpr int __in0 = min(int(sizeof(_U0)) * __CHAR_BIT__, __input_bits);
      constexpr int __in1 = __input_bits - __in0;
      if constexpr (__in1 == 0)
	return __duplicate_each_bit<__in0>(__x._M_first);
      else
	return __trivial_pair { __duplicate_each_bit<__in0>(__x._M_first),
				__duplicate_each_bit<__in1>(__x._M_second) };
    }

  template <__vec_builtin _TV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline _TV
    __x86_complex_multiplies(_TV __x, _TV __y)
    {
      using _Tp = __vec_value_type<_TV>;
      using _VO = _VecOps<_TV>;

      static_assert(_Traits._M_have_fma());
      static_assert(is_floating_point_v<_Tp>);

      if constexpr (!_Traits._M_have_avx512fp16() && sizeof(_Tp) == 2)
	return __vec_cast<_Tp>(__x86_complex_multiplies(__vec_cast<float>(__x),
							__vec_cast<float>(__y)));
      else if constexpr (sizeof(_TV) < 16)
	return _VO::_S_extract(__x86_complex_multiplies(__vec_zero_pad_to_16(__x),
							__vec_zero_pad_to_16(__y)));

      else
	{
	  _TV __x_real = _VO::_S_dup_even(__x);
	  _TV __x_imag = _VO::_S_dup_odd(__x);
	  _TV __y_swapped = _VO::_S_swap_neighbors(__y);

	  if constexpr (sizeof(__x) == 16 && sizeof(_Tp) == 2)
	    return __builtin_ia32_vfmaddsubph128_mask(__x_real, __y, __x_imag * __y_swapped, -1);
	  else if constexpr (sizeof(__x) == 32 && sizeof(_Tp) == 2)
	    return __builtin_ia32_vfmaddsubph256_mask(__x_real, __y, __x_imag * __y_swapped, -1);
	  else if constexpr (sizeof(__x) == 64 && sizeof(_Tp) == 2)
	    return __builtin_ia32_vfmaddsubph512_mask(
		     __x_real, __y, __x_imag * __y_swapped, -1, 0x04);

	  else if constexpr (sizeof(__x) == 16 && sizeof(_Tp) == 4)
	    return __builtin_ia32_vfmaddsubps(__x_real, __y, __x_imag * __y_swapped);
	  else if constexpr (sizeof(__x) == 32 && sizeof(_Tp) == 4)
	    return __builtin_ia32_vfmaddsubps256(__x_real, __y, __x_imag * __y_swapped);
	  else if constexpr (sizeof(__x) == 64 && sizeof(_Tp) == 4)
	    return __builtin_ia32_vfmaddsubps512_mask(
		     __x_real, __y, __x_imag * __y_swapped, -1, 0x04);

	  else if constexpr (sizeof(__x) == 16 && sizeof(_Tp) == 8)
	    return __builtin_ia32_vfmaddsubpd(__x_real, __y, __x_imag * __y_swapped);
	  else if constexpr (sizeof(__x) == 32 && sizeof(_Tp) == 8)
	    return __builtin_ia32_vfmaddsubpd256(__x_real, __y, __x_imag * __y_swapped);
	  else if constexpr (sizeof(__x) == 64 && sizeof(_Tp) == 8)
	    return __builtin_ia32_vfmaddsubpd512_mask(
		     __x_real, __y, __x_imag * __y_swapped, -1, 0x04);

	  else
	    static_assert(false);
	}
    }

  // FIXME: Work around PR121688
  template <__vec_builtin _UV, __vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    inline _UV
    __x86_cvt_f16c(_TV __v)
    {
      constexpr bool __from_f16 = is_same_v<__vec_value_type<_TV>, _Float16>;
      constexpr bool __to_f16 = !__from_f16;
      if constexpr (__to_f16 && !is_same_v<__vec_value_type<_TV>, float>)
	return __x86_cvt_f16c<_UV>(__vec_cast<float>(__v));
      else if constexpr (__from_f16 && !is_same_v<__vec_value_type<_UV>, float>)
	return __vec_cast<_UV>(__x86_cvt_f16c<__vec_builtin_type<float, __width_of<_TV>>>(__v));
      else if constexpr (__from_f16)
	{
	  const auto __vi = __vec_bit_cast<__x86_intrin_int<_Float16>>(__v);
	  if constexpr (sizeof(_TV) == 4)
	    return __vec_split_lo(__builtin_ia32_vcvtph2ps(__vec_zero_pad_to_16(__vi)));
	  else if constexpr (sizeof(_TV) == 8)
	    return __builtin_ia32_vcvtph2ps(__vec_zero_pad_to_16(__vi));
	  else if constexpr (sizeof(_TV) == 16)
	    return __builtin_ia32_vcvtph2ps256(__vi);
	  else if constexpr (sizeof(_TV) == 32)
	    return __builtin_ia32_vcvtph2ps512_mask(__vi, __vec_builtin_type<float, 16>(), -1, 4);
	  else if constexpr (sizeof(_TV) >= 64)
	    return __vec_concat(__x86_cvt_f16c<__half_vec_builtin_t<_UV>>(__vec_split_lo(__v)),
				__x86_cvt_f16c<__half_vec_builtin_t<_UV>>(__vec_split_hi(__v)));
	  else
	    static_assert(false);
	}
      else if constexpr (sizeof(_TV) == 8)
	return reinterpret_cast<_UV>(
		 __vec_split_lo(__vec_split_lo(__builtin_ia32_vcvtps2ph(
						 __vec_zero_pad_to_16(__v), 4))));
      else if constexpr (sizeof(_TV) == 16)
	return reinterpret_cast<_UV>(__vec_split_lo(__builtin_ia32_vcvtps2ph(__v, 4)));
      else if constexpr (sizeof(_TV) == 32)
	return reinterpret_cast<_UV>(__builtin_ia32_vcvtps2ph256(__v, 4));
      else if constexpr (sizeof(_TV) == 64)
	return reinterpret_cast<_UV>(__builtin_ia32_vcvtps2ph512_mask(
				       __v, 4, __vec_builtin_type<short, 16>(), -1));
      else if constexpr (sizeof(_TV) >= 128)
	return __vec_concat(__x86_cvt_f16c<__half_vec_builtin_t<_UV>>(__vec_split_lo(__v)),
			    __x86_cvt_f16c<__half_vec_builtin_t<_UV>>(__vec_split_hi(__v)));
      else
	static_assert(false);
    }

  /** @internal
   * AVX instructions typically work per 128-bit chunk. Horizontal operations thus produce vectors
   * where the two 128-bit chunks in the center are swapped. This function works as a fix-up step.
   */
  template <__vec_builtin _TV>
    [[__gnu__::__always_inline__]]
    inline _TV
    __x86_swizzle4x64_acbd(_TV __x)
    {
      static_assert(sizeof(_TV) == 32);
      using _UV = __vec_builtin_type_bytes<long long, 32>;
      return reinterpret_cast<_TV>(__builtin_shufflevector(reinterpret_cast<_UV>(__x), _UV(),
							   0, 2, 1, 3));
    }

  /** @internal
   * Like __builtin_convertvector but with a precondition that input values are either 0 or -1.
   */
  template <__vec_builtin _To, __vec_builtin _From>
    [[__gnu__::__always_inline__]]
    inline _To
    __x86_cvt_vecmask(_From __k)
    {
      using _T0 = __vec_value_type<_From>;
      using _T1 = __vec_value_type<_To>;
      if constexpr (sizeof(_From) > sizeof(_To) && sizeof(_From) < 16)
	{
	  using _ToPadded = __vec_builtin_type_bytes<_T1, sizeof(_To) * 16 / sizeof(_From)>;
	  return _VecOps<_To>::_S_extract(__x86_cvt_vecmask<_ToPadded>(__vec_zero_pad_to_16(__k)));
	}
      else if constexpr (sizeof(_T0) == 2 && sizeof(_T1) == 1) // -> packsswb
	{
	  if constexpr (sizeof(__k) == 16)
	    return reinterpret_cast<_To>(__vec_split_lo(__builtin_ia32_packsswb128(__k, __k)));
	  else if constexpr (sizeof(__k) == 32)
	    return reinterpret_cast<_To>(
		     __vec_split_lo(__x86_swizzle4x64_acbd(
				      __builtin_ia32_packsswb256(__k, __k))));
	  else
	    static_assert(false);
	}
      else
	static_assert(false, "TODO");
    }

  /** @internal
   * Overload that concatenates @p __k0 and @p __k1 while converting.
   */
  template <__vec_builtin _To, __vec_builtin _From>
    [[__gnu__::__always_inline__]]
    inline _To
    __x86_cvt_vecmask(_From __k0, _From __k1)
    {
      using _T0 = __vec_value_type<_From>;
      using _T1 = __vec_value_type<_To>;
      static_assert(sizeof(_From) >= 16);
      if constexpr (sizeof(_T0) == 2 && sizeof(_T1) == 1) // -> packsswb
	{
	  if constexpr (sizeof(__k0) == 16)
	    return reinterpret_cast<_To>(__builtin_ia32_packsswb128(__k0, __k1));
	  else if constexpr (sizeof(__k0) == 32)
	    return reinterpret_cast<_To>(__x86_swizzle4x64_acbd(
					   __builtin_ia32_packsswb256(__k0, __k1)));
	  else
	    static_assert(false);
	}
      else
	static_assert(false, "TODO");
    }

  /** @internal
   * AVX512 masked (converting) loads
   *
   * @note AVX512VL and AVX512BW is required
   */
  template <__vec_builtin _TV, typename _Up, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline _TV
    __x86_masked_load(const _Up* __mem, unsigned_integral auto __k)
    {
      static_assert(_Traits._M_have_avx512vl() && _Traits._M_have_avx512bw());
      using _Tp = __vec_value_type<_TV>;
      constexpr int __n = __width_of<_TV>;
      if constexpr (!__converts_trivially<_Up, _Tp>)
	{
	  const auto __uvec
	    = __x86_masked_load<__vec_builtin_type<__canonical_vec_type_t<_Up>, __n>>(__mem, __k);
	  return __vec_cast<_TV>(__uvec);
	}
      else if constexpr (sizeof(_TV) < 16)
	{
	  return _VecOps<_TV>::_S_extract(
		   __x86_masked_load<__vec_builtin_type_bytes<_Tp, 16>>(__mem, __k));
	}
      else if constexpr (sizeof(_TV) > 64)
	{
	  return __vec_concat(
		   __x86_masked_load<__vec_builtin_type<_Tp, __n / 2>>(__mem, __k),
		   __x86_masked_load<__vec_builtin_type<_Tp, __n / 2>>(__mem + __n / 2, __k >> __n / 2)
		 );
	}
      else if constexpr (sizeof(_TV) == 64)
	{
	  const auto* __src = reinterpret_cast<const __x86_intrin_type<_Up>*>(__mem);
	  const __vec_builtin_type_bytes<__x86_intrin_type<_Up>, 64> __z = {};
	  if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 4)
	    return __builtin_ia32_loadups512_mask(__src, __z, __k);
	  else if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 8)
	    return __builtin_ia32_loadupd512_mask(__src, __z, __k);
	  else if constexpr (sizeof(_Tp) == 1)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddquqi512_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 2)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddquhi512_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 4)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddqusi512_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 8)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddqudi512_mask(__src, __z, __k));
	  else
	    static_assert(false);
	}
      else if constexpr (sizeof(_TV) == 32)
	{
	  const auto* __src = reinterpret_cast<const __x86_intrin_type<_Up>*>(__mem);
	  const __vec_builtin_type_bytes<__x86_intrin_type<_Up>, 32> __z = {};
	  if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 4)
	    return __builtin_ia32_loadups256_mask(__src, __z, __k);
	  else if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 8)
	    return __builtin_ia32_loadupd256_mask(__src, __z, __k);
	  else if constexpr (sizeof(_Tp) == 1)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddquqi256_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 2)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddquhi256_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 4)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddqusi256_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 8)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddqudi256_mask(__src, __z, __k));
	  else
	    static_assert(false);
	}
      else if constexpr (sizeof(_TV) == 16)
	{
	  const auto* __src = reinterpret_cast<const __x86_intrin_type<_Up>*>(__mem);
	  const __vec_builtin_type_bytes<__x86_intrin_type<_Up>, 16> __z = {};
	  if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 4)
	    return __builtin_ia32_loadups128_mask(__src, __z, __k);
	  else if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 8)
	    return __builtin_ia32_loadupd128_mask(__src, __z, __k);
	  else if constexpr (sizeof(_Tp) == 1)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddquqi128_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 2)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddquhi128_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 4)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddqusi128_mask(__src, __z, __k));
	  else if constexpr (sizeof(_Tp) == 8)
	    return reinterpret_cast<_TV>(__builtin_ia32_loaddqudi128_mask(__src, __z, __k));
	  else
	    static_assert(false);
	}
      else
	static_assert(false);
    }

  /** @internal
   * AVX(2) masked loads (only trivial conversions)
   */
  template <__vec_builtin _TV, typename _Up, __vec_builtin _KV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline _TV
    __x86_masked_load(const _Up* __mem, const _KV __k)
    {
      using _Tp = __vec_value_type<_TV>;
      static_assert(_Traits._M_have_avx() && __converts_trivially<_Up, _Tp> && sizeof(_Up) >= 4);
      constexpr int __n = __width_of<_TV>;
      using _IV = __vec_builtin_type<__x86_intrin_int<_Tp>, __n>;
      const auto __vk = reinterpret_cast<_IV>(__k);
      if constexpr (sizeof(_TV) < 16)
	return _VecOps<_TV>::_S_extract(__x86_masked_load<__vec_builtin_type_bytes<_Tp, 16>>(
					  __mem, __vec_zero_pad_to_16(__k)));
      else if constexpr (_Traits._M_have_avx2() && is_integral_v<_Up>)
	{
	  const auto* __src
	    = reinterpret_cast<const __vec_builtin_type<__x86_intrin_int<_Up>, __n>*>(__mem);
	  if constexpr (sizeof(_Up) == 4 && sizeof(_TV) == 32)
	    return reinterpret_cast<_TV>(__builtin_ia32_maskloadd256(__src, __vk));
	  else if constexpr (sizeof(_Up) == 4 && sizeof(_TV) == 16)
	    return reinterpret_cast<_TV>(__builtin_ia32_maskloadd(__src, __vk));
	  else if constexpr (sizeof(_Up) == 8 && sizeof(_TV) == 32)
	    return reinterpret_cast<_TV>(__builtin_ia32_maskloadq256(__src, __vk));
	  else if constexpr (sizeof(_Up) == 8 && sizeof(_TV) == 16)
	    return reinterpret_cast<_TV>(__builtin_ia32_maskloadq(__src, __vk));
	  else
	    static_assert(false);
	}
      else if constexpr (sizeof(_Up) == 4)
	{
	  const auto* __src = reinterpret_cast<const __vec_builtin_type<float, __n>*>(__mem);
	  if constexpr (sizeof(_TV) == 32)
	    return reinterpret_cast<_TV>(__builtin_ia32_maskloadps256(__src, __vk));
	  else if constexpr (sizeof(_TV) == 16)
	    return reinterpret_cast<_TV>(__builtin_ia32_maskloadps(__src, __vk));
	  else
	    static_assert(false);
	}
      else
	{
	  const auto* __src = reinterpret_cast<const __vec_builtin_type<double, __n>*>(__mem);
	  if constexpr (sizeof(_TV) == 32)
	    return reinterpret_cast<_TV>(__builtin_ia32_maskloadpd256(__src, __vk));
	  else if constexpr (sizeof(_TV) == 16)
	    return reinterpret_cast<_TV>(__builtin_ia32_maskloadpd(__src, __vk));
	  else
	    static_assert(false);
	}
    }

  /** @internal
   * AVX512 masked stores
   *
   * @note AVX512VL is required
   */
  template <__vec_builtin _TV, typename _Up>
    [[__gnu__::__always_inline__]]
    inline void
    __x86_masked_store(const _TV __v, _Up* __mem, unsigned_integral auto __k)
    {
      using _Tp = __vec_value_type<_TV>;
      constexpr int __n = __width_of<_TV>;
      [[maybe_unused]] const auto __w = __vec_bit_cast<__x86_intrin_type<_Tp>>(__v);
      if constexpr (sizeof(_TV) == 64)
	{
	  if constexpr (sizeof(_Tp) > sizeof(_Up) && is_integral_v<_Tp> && is_integral_v<_Up>)
	    {
	      auto* __dst = reinterpret_cast<
			      __vec_builtin_type<__x86_intrin_int<_Up>, __n>*>(__mem);
	      if constexpr (sizeof(_Tp) == 2)
		__builtin_ia32_pmovwb512mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4 && sizeof(_Up) == 1)
		__builtin_ia32_pmovdb512mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4 && sizeof(_Up) == 2)
		__builtin_ia32_pmovdw512mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 1)
		__builtin_ia32_pmovqb512mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 2)
		__builtin_ia32_pmovqw512mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 4)
		__builtin_ia32_pmovqd512mem_mask(__dst, __w, __k);
	      else
		static_assert(false);
	    }
	  else if constexpr (__converts_trivially<_Tp, _Up>)
	    {
	      auto* __dst = reinterpret_cast<__x86_intrin_type<_Up>*>(__mem);
	      if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 4)
		__builtin_ia32_storeups512_mask(__dst, __w, __k);
	      else if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 8)
		__builtin_ia32_storeupd512_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 1)
		__builtin_ia32_storedquqi512_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 2)
		__builtin_ia32_storedquhi512_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4)
		__builtin_ia32_storedqusi512_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8)
		__builtin_ia32_storedqudi512_mask(__dst, __w, __k);
	      else
		static_assert(false);
	    }
	  else if constexpr (sizeof(_Tp) >= sizeof(_Up))
	    {
	      if constexpr (is_floating_point_v<_Tp> && is_integral_v<_Up>
			      && sizeof(_Tp) > sizeof(_Up))
		__x86_masked_store(__vec_cast<__integer_from<sizeof(_Tp)>>(__v), __mem, __k);
	      else
		__x86_masked_store(__vec_cast<_Up>(__v), __mem, __k);
	    }
	  else
	    {
	      __x86_masked_store(__vec_split_lo(__v), __mem, _Bitmask<__n / 2>(__k));
	      __x86_masked_store(__vec_split_hi(__v), __mem + __n / 2,
				 _Bitmask<__n / 2>(__k >> (__n / 2)));
	    }
	}
      else if constexpr (sizeof(_TV) == 32)
	{
	  if constexpr (sizeof(_Tp) > sizeof(_Up) && is_integral_v<_Tp> && is_integral_v<_Up>)
	    {
	      auto* __dst = reinterpret_cast<
			      __vec_builtin_type<__x86_intrin_int<_Up>, __n>*>(__mem);
	      if constexpr (sizeof(_Tp) == 2)
		__builtin_ia32_pmovwb256mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4 && sizeof(_Up) == 1)
		__builtin_ia32_pmovdb256mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4 && sizeof(_Up) == 2)
		__builtin_ia32_pmovdw256mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 1)
		__builtin_ia32_pmovqb256mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 2)
		__builtin_ia32_pmovqw256mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 4)
		__builtin_ia32_pmovqd256mem_mask(__dst, __w, __k);
	      else
		static_assert(false);
	    }
	  else if constexpr (__converts_trivially<_Tp, _Up>)
	    {
	      auto* __dst = reinterpret_cast<__x86_intrin_type<_Up>*>(__mem);
	      if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 4)
		__builtin_ia32_storeups256_mask(__dst, __w, __k);
	      else if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 8)
		__builtin_ia32_storeupd256_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 1)
		__builtin_ia32_storedquqi256_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 2)
		__builtin_ia32_storedquhi256_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4)
		__builtin_ia32_storedqusi256_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8)
		__builtin_ia32_storedqudi256_mask(__dst, __w, __k);
	      else
		static_assert(false);
	    }
	  else if constexpr (2 * sizeof(_Tp) >= sizeof(_Up))
	    {
	      __x86_masked_store(__vec_cast<_Up>(__v), __mem, __k);
	    }
	  else
	    {
	      __x86_masked_store(__vec_split_lo(__v), __mem, _Bitmask<__n / 2>(__k));
	      __x86_masked_store(__vec_split_hi(__v), __mem + __n / 2,
				 _Bitmask<__n / 2>(__k >> (__n / 2)));
	    }
	}
      else if constexpr (sizeof(_TV) == 16)
	{
	  if constexpr (sizeof(_Tp) > sizeof(_Up) && is_integral_v<_Tp> && is_integral_v<_Up>)
	    {
	      auto* __dst = reinterpret_cast<
			      __vec_builtin_type<__x86_intrin_int<_Up>, __n>*>(__mem);
	      if constexpr (sizeof(_Tp) == 2)
		__builtin_ia32_pmovwb128mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4 && sizeof(_Up) == 1)
		__builtin_ia32_pmovdb128mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4 && sizeof(_Up) == 2)
		__builtin_ia32_pmovdw128mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 1)
		__builtin_ia32_pmovqb128mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 2)
		__builtin_ia32_pmovqw128mem_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8 && sizeof(_Up) == 4)
		__builtin_ia32_pmovqd128mem_mask(reinterpret_cast<unsigned long long*>(__mem),
						 __w, __k);
	      else
		static_assert(false);
	    }
	  else if constexpr (__converts_trivially<_Tp, _Up>)
	    {
	      auto* __dst = reinterpret_cast<__x86_intrin_type<_Up>*>(__mem);
	      if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 4)
		__builtin_ia32_storeups128_mask(__dst, __w, __k);
	      else if constexpr (is_floating_point_v<_Tp> && sizeof(_Tp) == 8)
		__builtin_ia32_storeupd128_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 1)
		__builtin_ia32_storedquqi128_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 2)
		__builtin_ia32_storedquhi128_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 4)
		__builtin_ia32_storedqusi128_mask(__dst, __w, __k);
	      else if constexpr (sizeof(_Tp) == 8)
		__builtin_ia32_storedqudi128_mask(__dst, __w, __k);
	      else
		static_assert(false);
	    }
	  else if constexpr (4 * sizeof(_Tp) >= sizeof(_Up))
	    {
	      __x86_masked_store(__vec_cast<_Up>(__v), __mem, __k);
	    }
	  else
	    {
	      __x86_masked_store(__vec_cast<_Up>(__vec_split_lo(__v)), __mem,
				 _Bitmask<__n / 2>(__k));
	      __x86_masked_store(__vec_cast<_Up>(__vec_split_hi(__v)), __mem + __n / 2,
				 _Bitmask<__n / 2>(__k >> (__n / 2)));
	    }
	}
      else
	__x86_masked_store(__vec_zero_pad_to_16(__v), __mem, __k);
    }

  /** @internal
   * AVX(2) masked stores
   */
  template <__vec_builtin _TV, typename _Up, __vec_builtin _KV, _ArchTraits _Traits = {}>
    [[__gnu__::__always_inline__]]
    inline void
    __x86_masked_store(const _TV __v, _Up* __mem, const _KV __k)
    {
      using _Tp = __vec_value_type<_TV>;
      constexpr int __n = __width_of<_TV>;
      static_assert(sizeof(_Tp) == 4 || sizeof(_Tp) == 8);
      auto* __dst = reinterpret_cast<
		      __vec_builtin_type<__x86_intrin_type<_Up>, __n>*>(__mem);
      [[maybe_unused]] const auto __w = __vec_bit_cast<__x86_intrin_type<_Tp>>(__v);
      if constexpr (sizeof(_TV) < 16)
	__x86_masked_store(__vec_zero_pad_to_16(__v), __mem, __vec_zero_pad_to_16(__k));
      else if constexpr (_Traits._M_have_avx2() && is_integral_v<_Tp>)
	{
	  if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 4)
	    __builtin_ia32_maskstored256(__dst, __k, __w);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 4)
	    __builtin_ia32_maskstored(__dst, __k, __w);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 8)
	    __builtin_ia32_maskstoreq256(__dst, __k, __w);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 8)
	    __builtin_ia32_maskstoreq(__dst, __k, __w);
	  else
	    static_assert(false);
	}
      else
	{
	  if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 4)
	    __builtin_ia32_maskstoreps256(__dst, __k, __w);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 4)
	    __builtin_ia32_maskstoreps(__dst, __k, __w);
	  else if constexpr (sizeof(_TV) == 32 && sizeof(_Tp) == 8)
	    __builtin_ia32_maskstorepd256(__dst, __k, __w);
	  else if constexpr (sizeof(_TV) == 16 && sizeof(_Tp) == 8)
	    __builtin_ia32_maskstorepd(__dst, __k, __w);
	  else
	    static_assert(false);
	}
    }
} // namespace simd
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#pragma GCC diagnostic pop
#endif // C++26
#endif // _GLIBCXX_SIMD_X86_H
