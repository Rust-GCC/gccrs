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

#ifndef _GLIBCXX_SIMD_MASK_H
#define _GLIBCXX_SIMD_MASK_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#if __cplusplus >= 202400L

#include "simd_iterator.h"
#include "vec_ops.h"
#if _GLIBCXX_X86
#include "simd_x86.h"
#endif

#include <bit>
#include <bitset>

// psabi warnings are bogus because the ABI of the internal types never leaks into user code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi"

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace simd
{
  template <unsigned _Np>
    struct _SwapNeighbors
    {
      consteval unsigned
      operator()(unsigned __i, unsigned __size) const
      {
	if (__size % (2 * _Np) != 0)
	  __builtin_abort(); // swap_neighbors<N> permutation requires a multiple of 2N elements
	else if (std::has_single_bit(_Np))
	  return __i ^ _Np;
	else if (__i % (2 * _Np) >= _Np)
	  return __i - _Np;
	else
	  return __i + _Np;
      }
    };

  template <size_t _Np, size_t _Mp>
    constexpr auto
    __bitset_split(const bitset<_Mp>& __b)
    {
      constexpr auto __bits_per_word = __CHAR_BIT__ * __SIZEOF_LONG__;
      if constexpr (_Np % __bits_per_word == 0)
	{
	  struct _Tmp
	  {
	    bitset<_Np> _M_lo;
	    bitset<_Mp - _Np> _M_hi;
	  };
	  return __builtin_bit_cast(_Tmp, __b);
	}
      else
	{
	  constexpr auto __bits_per_ullong = __CHAR_BIT__ * __SIZEOF_LONG_LONG__;
	  static_assert(_Mp <= __bits_per_ullong);
	  using _Lo = _Bitmask<_Np>;
	  using _Hi = _Bitmask<_Mp - _Np>;
	  struct _Tmp
	  {
	    _Lo _M_lo;
	    _Hi _M_hi;
	  };
	  return _Tmp {static_cast<_Lo>(__b.to_ullong()), static_cast<_Hi>(__b.to_ullong() >> _Np)};
	}
    }

  static_assert(__bitset_split<64>(bitset<128>(1))._M_lo == bitset<64>(1));
  static_assert(__bitset_split<64>(bitset<128>(1))._M_hi == bitset<64>(0));

  // [simd.traits]
  // --- rebind ---
  template <typename _Tp, typename _Vp, _ArchTraits _Traits = {}>
    struct rebind
    {};

  /**
   * Computes a member @c type `basic_vec<_Tp, Abi>`, where @c Abi is chosen such that the
   * number of elements is equal to `_Vp::size()` and features of the ABI tag (such as the
   * internal representation of masks, or storage order of complex components) are preserved.
   */
  template <__vectorizable _Tp, __simd_vec_type _Vp, _ArchTraits _Traits>
    //requires requires { typename __deduce_abi_t<_Tp, _Vp::size()>; }
    struct rebind<_Tp, _Vp, _Traits>
    { using type = __similar_vec<_Tp, _Vp::size(), typename _Vp::abi_type>; };

  /**
   * As above, except for @c basic_mask.
   */
  template <__vectorizable _Tp, __simd_mask_type _Mp, _ArchTraits _Traits>
    //requires requires { typename __deduce_abi_t<_Tp, _Mp::size()>; }
    struct rebind<_Tp, _Mp, _Traits>
    { using type = __similar_mask<_Tp, _Mp::size(), typename _Mp::abi_type>; };

  template <typename _Tp, typename _Vp>
    using rebind_t = typename rebind<_Tp, _Vp>::type;

  // --- resize ---
  template <__simd_size_type _Np, typename _Vp, _ArchTraits _Traits = {}>
    struct resize
    {};

  template <__simd_size_type _Np, __simd_vec_type _Vp, _ArchTraits _Traits>
    requires (_Np >= 1)
    //requires requires { typename __deduce_abi_t<typename _Vp::value_type, _Np>; }
    struct resize<_Np, _Vp, _Traits>
    { using type = __similar_vec<typename _Vp::value_type, _Np, typename _Vp::abi_type>; };

  template <__simd_size_type _Np, __simd_mask_type _Mp, _ArchTraits _Traits>
    requires (_Np >= 1)
    //requires requires { typename __deduce_abi_t<typename _Mp::value_type, _Np>; }
    struct resize<_Np, _Mp, _Traits>
    {
      using _A1 = decltype(__abi_rebind<__mask_element_size<_Mp>, _Np, typename _Mp::abi_type,
					true>());

      static_assert(__abi_tag<_A1>);

      static_assert(_Mp::abi_type::_S_variant == _A1::_S_variant || __scalar_abi_tag<_A1>
		      || __scalar_abi_tag<typename _Mp::abi_type>);

      using type = basic_mask<__mask_element_size<_Mp>, _A1>;
    };

  template <__simd_size_type _Np, typename _Vp>
    using resize_t = typename resize<_Np, _Vp>::type;

  // [simd.syn]
  inline constexpr __simd_size_type zero_element   = numeric_limits<int>::min();

  inline constexpr __simd_size_type uninit_element = zero_element + 1;

  // [simd.permute.static]
  template<__simd_size_type _Np = 0, __simd_vec_or_mask_type _Vp,
	   __index_permutation_function<_Vp> _IdxMap>
    [[__gnu__::__always_inline__]]
    constexpr resize_t<_Np == 0 ? _Vp::size() : _Np, _Vp>
    permute(const _Vp& __v, _IdxMap&& __idxmap)
    { return resize_t<_Np == 0 ? _Vp::size() : _Np, _Vp>::_S_static_permute(__v, __idxmap); }

  // [simd.permute.dynamic]
  template<__simd_vec_or_mask_type _Vp, __simd_integral _Ip>
    [[__gnu__::__always_inline__]]
    constexpr resize_t<_Ip::size(), _Vp>
    permute(const _Vp& __v, const _Ip& __indices)
    { return __v[__indices]; }

  // [simd.creation] ----------------------------------------------------------
  template<__simd_vec_type _Vp, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr auto
    chunk(const basic_vec<typename _Vp::value_type, _Ap>& __x) noexcept
    { return __x.template _M_chunk<_Vp>(); }

  template<__simd_mask_type _Mp, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr auto
    chunk(const basic_mask<__mask_element_size<_Mp>, _Ap>& __x) noexcept
    { return __x.template _M_chunk<_Mp>(); }

  template<__simd_size_type _Np, typename _Tp, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr auto
    chunk(const basic_vec<_Tp, _Ap>& __x) noexcept
    -> decltype(chunk<resize_t<_Np, basic_vec<_Tp, _Ap>>>(__x))
    { return chunk<resize_t<_Np, basic_vec<_Tp, _Ap>>>(__x); }

  template<__simd_size_type _Np, size_t _Bytes, typename _Ap>
    [[__gnu__::__always_inline__]]
    constexpr auto
    chunk(const basic_mask<_Bytes, _Ap>& __x) noexcept
    -> decltype(chunk<resize_t<_Np, basic_mask<_Bytes, _Ap>>>(__x))
    { return chunk<resize_t<_Np, basic_mask<_Bytes, _Ap>>>(__x); }

  // LWG???? (reported 2025-11-25)
  template<typename _Tp, typename _A0, typename... _Abis>
    constexpr resize_t<(_A0::_S_size + ... + _Abis::_S_size), basic_vec<_Tp, _A0>>
    cat(const basic_vec<_Tp, _A0>& __x0, const basic_vec<_Tp, _Abis>&... __xs) noexcept
    {
      return resize_t<(_A0::_S_size + ... + _Abis::_S_size), basic_vec<_Tp, _A0>>
	       ::_S_concat(__x0, __xs...);
    }

  // LWG???? (reported 2025-11-25)
  template<size_t _Bytes, typename _A0, typename... _Abis>
    constexpr resize_t<(_A0::_S_size + ... + _Abis::_S_size), basic_mask<_Bytes, _A0>>
    cat(const basic_mask<_Bytes, _A0>& __x0, const basic_mask<_Bytes, _Abis>&... __xs) noexcept
    {
      return resize_t<(_A0::_S_size + ... + _Abis::_S_size), basic_mask<_Bytes, _A0>>
	       ::_S_concat(__x0, __xs...);
    }

  // implementation helper for chunk and cat
  consteval int
  __packs_to_skip_at_front(int __offset, initializer_list<int> __sizes)
  {
    int __i = 0;
    int __n = 0;
    for (int __s : __sizes)
      {
	__n += __s;
	if (__n > __offset)
	  return __i;
	++__i;
      }
    __builtin_trap(); // called out of contract
  }

  consteval int
  __packs_to_skip_at_back(int __offset, int __max, initializer_list<int> __sizes)
  {
    int __i = 0;
    int __n = -__offset;
    for (int __s : __sizes)
      {
	++__i;
	__n += __s;
	if (__n >= __max)
	  return int(__sizes.size()) - __i;
      }
    return 0;
  }

  // in principle, this overload allows conversions to _Dst - and it wouldn't be wrong - but the
  // general overload below is still a better candidate in overload resolution
  template <typename _Dst>
    [[__gnu__::__always_inline__]]
    constexpr _Dst
    __extract_simd_at(auto _Offset, const _Dst& __r, const auto&...)
    requires(_Offset.value == 0)
    { return __r; }

  template <typename _Dst, typename _V0>
    [[__gnu__::__always_inline__]]
    constexpr _Dst
    __extract_simd_at(auto _Offset, const _V0&, const _Dst& __r, const auto&...)
    requires(_Offset.value == _V0::size.value)
    { return __r; }

  template <typename _Dst, typename... _Vs>
    [[__gnu__::__always_inline__]]
    constexpr _Dst
    __extract_simd_at(auto _Offset, const _Vs&... __xs)
    {
      using _Adst = typename _Dst::abi_type;
      if constexpr (_Adst::_S_nreg >= 2)
	{
	  using _Dst0 = remove_cvref_t<decltype(declval<_Dst>()._M_get_low())>;
	  using _Dst1 = remove_cvref_t<decltype(declval<_Dst>()._M_get_high())>;
	  return _Dst::_S_init(__extract_simd_at<_Dst0>(_Offset, __xs...),
			       __extract_simd_at<_Dst1>(_Offset + _Dst0::size, __xs...));
	}
      else
	{
	  using _Ret = remove_cvref_t<decltype(declval<_Dst>()._M_get())>;
	  constexpr bool __use_bitmask = __simd_mask_type<_Dst> && _Adst::_S_is_bitmask;
	  constexpr int __dst_full_size = __bit_ceil(unsigned(_Adst::_S_size));
	  constexpr int __nargs = sizeof...(__xs);
	  using _Afirst = typename _Vs...[0]::abi_type;
	  using _Alast = typename _Vs...[__nargs - 1]::abi_type;
	  const auto& __x0 = __xs...[0];
	  const auto& __xlast = __xs...[__nargs - 1];
	  constexpr int __ninputs = (_Vs::size.value + ...);
	  if constexpr (_Offset.value >= _Afirst::_S_size
			  || __ninputs - _Offset.value - _Alast::_S_size >= _Adst::_S_size)
	    { // can drop inputs at the front and/or back of the pack
	      constexpr int __skip_front = __packs_to_skip_at_front(_Offset.value,
								    {_Vs::size.value...});
	      constexpr int __skip_back = __packs_to_skip_at_back(_Offset.value, _Adst::_S_size,
								  {_Vs::size.value...});
	      static_assert(__skip_front > 0 || __skip_back > 0);
	      constexpr auto [...__skip] = _IotaArray<__skip_front>;
	      constexpr auto [...__is] = _IotaArray<__nargs - __skip_front - __skip_back>;
	      constexpr int __new_offset = _Offset.value - (0 + ... + _Vs...[__skip]::size.value);
	      return __extract_simd_at<_Dst>(cw<__new_offset>, __xs...[__is + __skip_front]...);
	    }
	  else if constexpr (_Adst::_S_size == 1)
	    { // trivial conversion to one value_type
	      return _Dst(__x0[_Offset.value]);
	    }
	  else if constexpr (_Afirst::_S_nreg >= 2 || _Alast::_S_nreg >= 2)
	    { // flatten first and/or last multi-register argument
	      constexpr bool __flatten_first = _Afirst::_S_nreg >= 2;
	      constexpr bool __flatten_last = __nargs > 1 && _Alast::_S_nreg >= 2;
	      constexpr auto [...__is] = _IotaArray<__nargs - __flatten_first - __flatten_last>;
	      if constexpr (__flatten_first && __flatten_last)
		return __extract_simd_at<_Dst>(
			 _Offset, __x0._M_get_low(), __x0._M_get_high(), __xs...[__is + 1]...,
			 __xlast._M_get_low(), __xlast._M_get_high());
	      else if constexpr (__flatten_first)
		return __extract_simd_at<_Dst>(
			 _Offset, __x0._M_get_low(), __x0._M_get_high(), __xs...[__is + 1]...);
	      else
		return __extract_simd_at<_Dst>(
			 _Offset, __xs...[__is]..., __xlast._M_get_low(), __xlast._M_get_high());
	    }
	  else if constexpr (__simd_mask_type<_Dst>
			       && ((_Adst::_S_variant != _Vs::abi_type::_S_variant
				      && !__scalar_abi_tag<typename _Vs::abi_type>) || ...))
	    { // convert ABI tag if incompatible
	      return __extract_simd_at<_Dst>(
		       _Offset, static_cast<const resize_t<_Vs::size.value, _Dst>&>(__xs)...);
	    }

	  // at this point __xs should be as small as possible; there may be some corner cases left

	  else if constexpr (__nargs == 1)
	    { // simple and optimal
	      if constexpr (__use_bitmask)
		return _Dst(_Ret(__x0._M_to_uint() >> _Offset.value));
	      else
		return _VecOps<_Ret>::_S_extract(__x0._M_concat_data(false), _Offset);
	    }
	  else if constexpr (__use_bitmask)
	    { // fairly simple and optimal bit shifting solution
	      static_assert(_Afirst::_S_nreg == 1);
	      static_assert(_Offset.value < _Afirst::_S_size);
	      int __offset = -_Offset.value;
	      _Ret __r;
	      template for (const auto& __x : {__xs...})
		{
		  if (__offset <= 0)
		    __r = _Ret(__x._M_to_uint() >> -__offset);
		  else if (__offset < _Adst::_S_size)
		    __r |= _Ret(_Ret(__x._M_to_uint()) << __offset);
		  __offset += __x.size.value;
		}
	      return _Dst(__r);
	    }
	  else if constexpr (__nargs == 2 && _Offset == 0 && _Adst::_S_nreg == 1
			       && _Afirst::_S_size >= _Alast::_S_size
			       && __has_single_bit(unsigned(_Afirst::_S_size)))
	    { // simple __vec_concat
	      if constexpr (_Afirst::_S_size == 1)
		// even simpler init from two values
		return _Ret{__x0._M_concat_data()[0], __xlast._M_concat_data()[0]};
	      else
		{
		  const auto __v0 = __x0._M_concat_data();
		  const auto __v1 = __vec_zero_pad_to<sizeof(__v0)>(__xlast._M_concat_data());
		  return __vec_concat(__v0, __v1);
		}
	    }
	  else if constexpr (__nargs == 2 && _Adst::_S_nreg == 1 && _Offset == 0
			       && _Afirst::_S_nreg == 1 && _Alast::_S_size == 1)
	    { // optimize insertion of one element at the end
	      _Ret __r = __vec_zero_pad_to<sizeof(_Ret)>(__x0._M_get());
	      __vec_set(__r, _Afirst::_S_size, __xlast._M_concat_data()[0]);
	      return __r;
	    }
	  else if constexpr (__nargs == 2 && _Adst::_S_nreg == 1 && _Offset == 0
			       && _Afirst::_S_nreg == 1 && _Alast::_S_size == 2)
	    { // optimize insertion of two elements at the end
	      _Ret __r = __vec_zero_pad_to<sizeof(_Ret)>(__x0._M_concat_data());
	      const auto __x1 = __xlast._M_concat_data();
	      if constexpr (sizeof(__x1) <= sizeof(double) && (_Afirst::_S_size & 1) == 0)
		{ // can use a single insert instruction
		  using _Up = __conditional_t<
				is_floating_point_v<__vec_value_type<_Ret>>,
				__conditional_t<sizeof(__x1) == sizeof(double), double, float>,
				__integer_from<sizeof(__x1)>>;
		  auto __r2 = __vec_bit_cast<_Up>(__r);
		  __vec_set(__r2, _Afirst::_S_size / 2, __vec_bit_cast<_Up>(__x1)[0]);
		  __r = reinterpret_cast<_Ret>(__r2);
		}
	      else
		{
		  __vec_set(__r, _Afirst::_S_size, __x1[0]);
		  __vec_set(__r, _Afirst::_S_size + 1, __x1[1]);
		}
	      return __r;
	    }
	  else if constexpr (__nargs == 2 && _Afirst::_S_nreg == 1 && _Alast::_S_nreg == 1)
	    { // optimize concat of two input vectors (e.g. using palignr)
	      constexpr auto [...__is] = _IotaArray<__dst_full_size>;
	      constexpr int __v2_offset = __width_of<decltype(__x0._M_concat_data())>;
	      return __builtin_shufflevector(
		       __x0._M_concat_data(), __xlast._M_concat_data(), [](int __i) consteval {
		       if (__i < _Afirst::_S_size)
			 return __i;
		       __i -= _Afirst::_S_size;
		       if (__i < _Alast::_S_size)
			 return __i + __v2_offset;
		       else
			 return -1;
		     }(__is + _Offset.value)...);
	    }
	  else if (__is_const_known(__xs...) || __ninputs == _Adst::_S_size)
	    { // hard to optimize for the compiler, but necessary in constant expressions
	      return _VecOps<_Ret>::_S_extract(
		       __vec_concat_sized<__xs.size.value...>(__xs._M_concat_data(false)...),
		       _Offset);
	    }
	  else
	    { // fallback to concatenation in memory => load the result
	      alignas(_Ret) __vec_value_type<_Ret>
		__tmp[std::max(__ninputs, _Offset.value + __dst_full_size)] = {};
	      int __offset = 0;
	      template for (const auto& __x : {__xs...})
		{
		  if constexpr (__simd_mask_type<_Dst>)
		    (-__x)._M_store(__tmp + __offset);
		  else
		    __x._M_store(__tmp + __offset);
		  __offset += __x.size.value;
		}
	      _Ret __r;
	      __builtin_memcpy(&__r, __tmp + _Offset.value, sizeof(_Ret));
	      return __r;
	    }
	}
    }

  // [simd.mask] --------------------------------------------------------------
  template <size_t _Bytes, typename _Ap>
    class basic_mask
    {
    public:
      using value_type = bool;

      using abi_type = _Ap;

#define _GLIBCXX_DELETE_SIMD "This specialization is disabled because of an invalid combination "  \
			     "of template arguments to basic_mask."

      basic_mask() = delete(_GLIBCXX_DELETE_SIMD);

      ~basic_mask() = delete(_GLIBCXX_DELETE_SIMD);

      basic_mask(const basic_mask&) = delete(_GLIBCXX_DELETE_SIMD);

      basic_mask& operator=(const basic_mask&) = delete(_GLIBCXX_DELETE_SIMD);

#undef _GLIBCXX_DELETE_SIMD
    };

  template <size_t _Bytes, typename _Ap>
    class _MaskBase
    {
      using _Mp = basic_mask<_Bytes, _Ap>;

    protected:
      using _VecType = __simd_vec_from_mask_t<_Bytes, _Ap>;

      static_assert(destructible<_VecType> || _Bytes > sizeof(0ull));

    public:
      using iterator = __iterator<_Mp>;

      using const_iterator = __iterator<const _Mp>;

      constexpr iterator
      begin() noexcept
      { return {static_cast<_Mp&>(*this), 0}; }

      constexpr const_iterator
      begin() const noexcept
      { return cbegin(); }

      constexpr const_iterator
      cbegin() const noexcept
      { return {static_cast<const _Mp&>(*this), 0}; }

      constexpr default_sentinel_t
      end() const noexcept
      { return {}; }

      constexpr default_sentinel_t
      cend() const noexcept
      { return {}; }

      static constexpr auto size = __simd_size_c<_Ap::_S_size>;

      _MaskBase() = default;

      // LWG issue from 2026-03-04 / P4042R0
      template <size_t _UBytes, typename _UAbi>
	requires (_Ap::_S_size != _UAbi::_S_size)
	explicit
	_MaskBase(const basic_mask<_UBytes, _UAbi>&) = delete("size mismatch");

      template <typename _Up, typename _UAbi>
	explicit
	_MaskBase(const basic_vec<_Up, _UAbi>&)
	  = delete("use operator! or a comparison to convert a vec into a mask");

      template <typename _Up, typename _UAbi>
	requires (_Ap::_S_size != _UAbi::_S_size)
	operator basic_vec<_Up, _UAbi>() const
	  = delete("size mismatch");
    };

  template <size_t _Bytes, __abi_tag _Ap>
    requires (_Ap::_S_nreg == 1)
    class basic_mask<_Bytes, _Ap>
    : public _MaskBase<_Bytes, _Ap>
    {
      using _Base = _MaskBase<_Bytes, _Ap>;

      using _VecType = _Base::_VecType;

      template <size_t, typename>
	friend class basic_mask;

      template <typename, typename>
	friend class basic_vec;

      static constexpr int _S_size = _Ap::_S_size;

      using _DataType = typename _Ap::template _MaskDataType<_Bytes>;

      static constexpr bool _S_has_bool_member = is_same_v<_DataType, bool>;

      static constexpr bool _S_is_scalar = _S_has_bool_member;

      static constexpr bool _S_use_bitmask = _Ap::_S_is_bitmask;

      static constexpr int _S_full_size = [] {
	if constexpr (_S_is_scalar)
	  return _S_size;
	else if constexpr (_S_use_bitmask && _S_size < __CHAR_BIT__)
	  return __CHAR_BIT__;
	else
	  return __bit_ceil(unsigned(_S_size));
      }();

      static constexpr bool _S_is_partial = _S_size != _S_full_size;

      static constexpr _DataType _S_implicit_mask = [] {
	if constexpr (_S_is_scalar)
	  return true;
	else if (!_S_is_partial)
	  return _DataType(~_DataType());
	else if constexpr (_S_use_bitmask)
	  return _DataType((_DataType(1) << _S_size) - 1);
	else
	  {
	    constexpr auto [...__is] = _IotaArray<_S_full_size>;
	    return _DataType{ (__is < _S_size ? -1 : 0)... };
	  }
      }();

      // Actual padding bytes, not padding elements.
      // => _S_padding_bytes is 0 even if _S_is_partial is true.
      static constexpr size_t _S_padding_bytes = 0;

      _DataType _M_data;

    public:
      using value_type = bool;

      using abi_type = _Ap;

      using iterator = _Base::iterator;

      using const_iterator = _Base::const_iterator;

      // internal but public API ----------------------------------------------
      [[__gnu__::__always_inline__]]
      static constexpr basic_mask
      _S_init(_DataType __x)
      {
	basic_mask __r;
	__r._M_data = __x;
	return __r;
      }

      [[__gnu__::__always_inline__]]
      static constexpr basic_mask
      _S_init(unsigned_integral auto __bits)
      { return basic_mask(__bits); }

      [[__gnu__::__always_inline__]]
      constexpr const _DataType&
      _M_get() const
      { return _M_data; }

      /** @internal
       * Bit-cast the given object @p __x to basic_mask.
       *
       * This is necessary for _S_nreg > 1 where the last element can be bool or when the sizeof
       * doesn't match because of different alignment requirements of the sub-masks.
       */
      template <size_t _UBytes, typename _UAbi>
	[[__gnu__::__always_inline__]]
	static constexpr basic_mask
	_S_recursive_bit_cast(const basic_mask<_UBytes, _UAbi>& __x)
	{ return __builtin_bit_cast(basic_mask, __x._M_concat_data()); }

      [[__gnu__::__always_inline__]]
      constexpr auto
      _M_concat_data(bool __do_sanitize = _S_is_partial) const
      {
	if constexpr (_S_is_scalar)
	  return __vec_builtin_type<__integer_from<_Bytes>, 1>{__integer_from<_Bytes>(-_M_data)};
	else
	  {
	    if constexpr (_S_is_partial)
	      if (__do_sanitize)
		return _DataType(_M_data & _S_implicit_mask);
	    return _M_data;
	  }
      }

      /** @internal
       * Returns a mask where the first @p __n elements are true. All remaining elements are false.
       *
       * @pre @p __n > 0 && @p __n < _S_size
       */
      template <_ArchTraits _Traits = {}>
	[[__gnu__::__always_inline__]]
	static constexpr basic_mask
	_S_partial_mask_of_n(int __n)
	{
	  static_assert(!_S_is_scalar);
	  if constexpr (!_S_use_bitmask)
	    {
	      using _Ip = __integer_from<_Bytes>;
	      __glibcxx_simd_precondition(__n >= 0 && __n <= numeric_limits<_Ip>::max(),
					  "_S_partial_mask_of_n without _S_use_bitmask requires "
					  "positive __n that does not overflow.");
	      constexpr _DataType __0123
		= __builtin_bit_cast(_DataType, _IotaArray<_Ip(_S_full_size)>);
	      return basic_mask(__0123 < _Ip(__n));
	    }
	  else
	    {
	      __glibcxx_simd_precondition(__n >= 0 && __n <= 255,
					  "The x86 BZHI instruction requires __n to "
					  "only use bits 0:7");
#if __has_builtin(__builtin_ia32_bzhi_si)
	      if constexpr (_S_size <= 32 && _Traits._M_have_bmi2())
		return _S_init(_Bitmask<_S_size>(
				 __builtin_ia32_bzhi_si(~0u >> (32 - _S_size), unsigned(__n))));
#endif
#if __has_builtin(__builtin_ia32_bzhi_di)
	      else if constexpr (_S_size <= 64 && _Traits._M_have_bmi2())
		return _S_init(__builtin_ia32_bzhi_di(~0ull >> (64 - _S_size), unsigned(__n)));
#endif
	      if constexpr (_S_size <= 32)
		{
		  __glibcxx_simd_precondition(__n < 32, "invalid shift");
		  return _S_init(_Bitmask<_S_size>((1u << unsigned(__n)) - 1));
		}
	      else if constexpr (_S_size <= 64)
		{
		  __glibcxx_simd_precondition(__n < 64, "invalid shift");
		  return _S_init((1ull << unsigned(__n)) - 1);
		}
	      else
		static_assert(false);
	    }
	}

      [[__gnu__::__always_inline__]]
      constexpr basic_mask&
      _M_and_neighbors()
      {
	if constexpr (_S_use_bitmask)
	  _M_data &= ((_M_data >> 1) & 0x5555'5555'5555'5555ull)
		       | ((_M_data << 1) & ~0x5555'5555'5555'5555ull);
	else
	  _M_data &= _VecOps<_DataType>::_S_swap_neighbors(_M_data);
	return *this;
      }

      [[__gnu__::__always_inline__]]
      constexpr basic_mask&
      _M_or_neighbors()
      {
	if constexpr (_S_use_bitmask)
	  _M_data |= ((_M_data >> 1) & 0x5555'5555'5555'5555ull)
		       | ((_M_data << 1) & ~0x5555'5555'5555'5555ull);
	else
	  _M_data |= _VecOps<_DataType>::_S_swap_neighbors(_M_data);
	return *this;
      }

      template <typename _Mp>
	[[__gnu__::__always_inline__]]
	constexpr auto _M_chunk() const noexcept
	{
	  constexpr int __n = _S_size / _Mp::_S_size;
	  constexpr int __rem = _S_size % _Mp::_S_size;
	  constexpr auto [...__is] = _IotaArray<__n>;
	  if constexpr (__rem == 0)
	    return array<_Mp, __n>{__extract_simd_at<_Mp>(cw<_Mp::_S_size * __is>, *this)...};
	  else
	    {
	      using _Rest = resize_t<__rem, _Mp>;
	      return tuple(__extract_simd_at<_Mp>(cw<_Mp::_S_size * __is>, *this)...,
			   __extract_simd_at<_Rest>(cw<_Mp::_S_size * __n>, *this));
	    }
	}

      [[__gnu__::__always_inline__]]
      static constexpr const basic_mask&
      _S_concat(const basic_mask& __x0) noexcept
      { return __x0; }

      template <typename... _As>
	requires (sizeof...(_As) > 1)
	[[__gnu__::__always_inline__]]
	static constexpr basic_mask
	_S_concat(const basic_mask<_Bytes, _As>&... __xs) noexcept
	{
	  static_assert(_S_size == (_As::_S_size + ...));
	  return __extract_simd_at<basic_mask>(cw<0>, __xs...);
	}

      // [simd.mask.overview] default constructor -----------------------------
      basic_mask() = default;

      // [simd.mask.overview] conversion extensions ---------------------------
      [[__gnu__::__always_inline__]]
      constexpr
      basic_mask(_DataType __x) requires(!_S_is_scalar && !_S_use_bitmask)
	: _M_data(__x)
      {}

      [[__gnu__::__always_inline__]]
      constexpr
      operator _DataType() requires(!_S_is_scalar && !_S_use_bitmask)
      { return _M_data; }

      // [simd.mask.ctor] broadcast constructor -------------------------------
      [[__gnu__::__always_inline__]]
      constexpr explicit
      basic_mask(same_as<bool> auto __x) noexcept // LWG 4382.
	: _M_data(__x ? _S_implicit_mask : _DataType())
      {}

      // [simd.mask.ctor] conversion constructor ------------------------------
      template <size_t _UBytes, typename _UAbi>
	requires (_S_size == _UAbi::_S_size)
	[[__gnu__::__always_inline__]]
	constexpr explicit(__is_mask_conversion_explicit<_Ap, _UAbi>(_Bytes, _UBytes))
	basic_mask(const basic_mask<_UBytes, _UAbi>& __x) noexcept
	  : _M_data([&] [[__gnu__::__always_inline__]] {
	      using _UV = basic_mask<_UBytes, _UAbi>;
	      // bool to bool
	      if constexpr (_S_is_scalar)
		return __x[0];

	      // converting from an "array of bool"
	      else if constexpr (_UV::_S_is_scalar)
		{
		  constexpr auto [...__is] = _IotaArray<_S_size>;
		  if constexpr (_S_use_bitmask)
		    return ((_DataType(__x[__is]) << __is) | ...);
		  else
		    return _DataType{__vec_value_type<_DataType>(-__x[__is])...};
		}

	      // vec-/bit-mask to bit-mask | bit-mask to vec-mask
	      else if constexpr (_S_use_bitmask || _UV::_S_use_bitmask)
		return basic_mask(__x.to_bitset())._M_data;

	      // vec-mask to vec-mask
	      else if constexpr (_Bytes == _UBytes)
		return _S_recursive_bit_cast(__x)._M_data;

	      else
		{
#if _GLIBCXX_X86
		  // TODO: turn this into a __vec_mask_cast overload in simd_x86.h
		  if constexpr (_Bytes == 1 && _UBytes == 2)
		    if (!__is_const_known(__x))
		      {
			if constexpr (_UAbi::_S_nreg == 1)
			  return __x86_cvt_vecmask<_DataType>(__x._M_data);
			else if constexpr (_UAbi::_S_nreg == 2)
			  {
			    auto __lo = __x._M_data0._M_data;
			    auto __hi = __vec_zero_pad_to<sizeof(__lo)>(
					  __x._M_data1._M_concat_data());
			    return __x86_cvt_vecmask<_DataType>(__lo, __hi);
			  }
		      }
#endif
		  return __vec_mask_cast<_DataType>(__x._M_concat_data());
		}
	  }())
	{}

      using _Base::_MaskBase;

      // [simd.mask.ctor] generator constructor -------------------------------
      template <__simd_generator_invokable<bool, _S_size> _Fp>
	[[__gnu__::__always_inline__]]
	constexpr explicit
	basic_mask(_Fp&& __gen)
	  : _M_data([&] [[__gnu__::__always_inline__]] {
	      constexpr auto [...__is] = _IotaArray<_S_size>;
	      if constexpr (_S_is_scalar)
		return __gen(__simd_size_c<0>);
	      else if constexpr (_S_use_bitmask)
		return _DataType(((_DataType(__gen(__simd_size_c<__is>)) << __is)
				    | ...));
	      else
		return _DataType{__vec_value_type<_DataType>(
				   __gen(__simd_size_c<__is>) ? -1 : 0)...};
	    }())
	{}

      // [simd.mask.ctor] bitset constructor ----------------------------------
      [[__gnu__::__always_inline__]]
      constexpr
      basic_mask(const same_as<bitset<_S_size>> auto& __b) noexcept // LWG 4382.
      : basic_mask(static_cast<_Bitmask<_S_size>>(__b.to_ullong()))
      {
	// more than 64 elements in one register? not yet.
	static_assert(_S_size <= numeric_limits<unsigned long long>::digits);
      }

      // [simd.mask.ctor] uint constructor ------------------------------------
      template <unsigned_integral _Tp>
	requires (!same_as<_Tp, bool>) // LWG 4382.
	[[__gnu__::__always_inline__]]
	constexpr explicit
	basic_mask(_Tp __val) noexcept
	: _M_data([&] [[__gnu__::__always_inline__]] () {
	    if constexpr (_S_use_bitmask)
	      return __val;
	    else if constexpr (_S_is_scalar)
	      return bool(__val & 1);
	    else if (__is_const_known(__val))
	      {
		constexpr auto [...__is] = _IotaArray<_S_size>;
		return _DataType {__vec_value_type<_DataType>((__val & (1ull << __is)) == 0
								? 0 : -1)...};
	      }
	    else
	      {
		using _Ip = typename _VecType::value_type;
		_VecType __v0 = _Ip(__val);
		constexpr int __bits_per_element = sizeof(_Ip) * __CHAR_BIT__;
		constexpr _VecType __pow2 = _VecType(1) << (__iota<_VecType> % __bits_per_element);
		if constexpr (_S_size < __bits_per_element)
		  return ((__v0 & __pow2) > 0)._M_concat_data();
		else if constexpr (_S_size == __bits_per_element)
		  return ((__v0 & __pow2) != 0)._M_concat_data();
		else
		  {
		    static_assert(_Bytes == 1);
		    static_assert(sizeof(_Ip) == 1);
		    _Bitmask<_S_size> __bits = __val;
		    static_assert(sizeof(_VecType) % sizeof(__bits) == 0);
		    if constexpr (sizeof(_DataType) == 32)
		      {
			__vec_builtin_type<_UInt<8>, 4> __v1 = {
			  0xffu & (__bits >> (0 * __CHAR_BIT__)),
			  0xffu & (__bits >> (1 * __CHAR_BIT__)),
			  0xffu & (__bits >> (2 * __CHAR_BIT__)),
			  0xffu & (__bits >> (3 * __CHAR_BIT__)),
			};
			__v1 *= 0x0101'0101'0101'0101ull;
			__v0 = __builtin_bit_cast(_VecType, __v1);
			return ((__v0 & __pow2) != 0)._M_data;
		      }
		    else
		      {
			using _V1 = vec<_Ip, sizeof(__bits)>;
			_V1 __v1 = __builtin_bit_cast(_V1, __bits);
			__v0 = _VecType::_S_static_permute(__v1, [](int __i) {
				 return __i / __CHAR_BIT__;
			       });
			return ((__v0 & __pow2) != 0)._M_data;
		      }
		  }
	      }
	  }())
	{}

      //Effects: Initializes the first M elements to the corresponding bit values in val, where M is
      //the smaller of size() and the number of bits in the value representation
      //([basic.types.general]) of the type of val. If M is less than size(), the remaining elements
      //are initialized to zero.


      // [simd.mask.subscr] ---------------------------------------------------
      [[__gnu__::__always_inline__]]
      constexpr value_type
      operator[](__simd_size_type __i) const
      {
	__glibcxx_simd_precondition(__i >= 0 && __i < _S_size, "subscript is out of bounds");
	if constexpr (_S_is_scalar)
	  return _M_data;
	else if constexpr (_S_use_bitmask)
	  return bool((_M_data >> __i) & 1);
	else
	  return _M_data[__i] & 1;
      }

      // [simd.mask.unary] ----------------------------------------------------
      [[__gnu__::__always_inline__]]
      constexpr basic_mask
      operator!() const noexcept
      {
	if constexpr (_S_is_scalar)
	  return _S_init(!_M_data);
	else
	  return _S_init(~_M_data);
      }

      [[__gnu__::__always_inline__]]
      constexpr _VecType
      operator+() const noexcept requires destructible<_VecType>
      { return operator _VecType(); }

      constexpr _VecType
      operator+() const noexcept = delete;

      [[__gnu__::__always_inline__]]
      constexpr _VecType
      operator-() const noexcept requires destructible<_VecType>
      {
	using _Ip = typename _VecType::value_type;
	if constexpr (_S_is_scalar)
	  return _Ip(-int(_M_data));
	else if constexpr (_S_use_bitmask)
	  return __select_impl(*this, _Ip(-1), _Ip());
	else
	  {
	    static_assert(sizeof(_VecType) == sizeof(_M_data));
	    return __builtin_bit_cast(_VecType, _M_data);
	  }
      }

      constexpr _VecType
      operator-() const noexcept = delete;

      [[__gnu__::__always_inline__]]
      constexpr _VecType
      operator~() const noexcept requires destructible<_VecType>
      {
	using _Ip = typename _VecType::value_type;
	if constexpr (_S_is_scalar)
	  return _Ip(~int(_M_data));
	else if constexpr (_S_use_bitmask)
	  return __select_impl(*this, _Ip(-2), _Ip(-1));
	else
	  {
	    static_assert(sizeof(_VecType) == sizeof(_M_data));
	    return __builtin_bit_cast(_VecType, _M_data) - _Ip(1);
	  }
      }

      constexpr _VecType
      operator~() const noexcept = delete;

      // [simd.mask.conv] -----------------------------------------------------
      template <typename _Up, typename _UAbi>
	requires (_UAbi::_S_size == _S_size)
	[[__gnu__::__always_inline__]]
	constexpr explicit(sizeof(_Up) != _Bytes)
	operator basic_vec<_Up, _UAbi>() const noexcept
	{
	  if constexpr (_S_is_scalar)
	    return _Up(_M_data);
	  else
	    {
	      using _UV = basic_vec<_Up, _UAbi>;
	      return __select_impl(static_cast<_UV::mask_type>(*this), _UV(1), _UV(0));
	    }
	}

      using _Base::operator basic_vec;

      // [simd.mask.namedconv] ------------------------------------------------
      [[__gnu__::__always_inline__]]
      constexpr bitset<_S_size>
      to_bitset() const noexcept
      {
	// more than 64 elements in one register? not yet.
	static_assert(_S_size <= numeric_limits<unsigned long long>::digits);
	return to_ullong();
      }

      /** @internal
       * Return the mask as the smallest possible unsigned integer (up to 64 bits).
       *
       * @tparam _Offset       Adjust the return type & value to start at bit @p _Offset.
       * @tparam _Use_2_for_1  Store the value of every second element into one bit of the result.
       *                       (precondition: each even/odd pair stores the same value)
       */
      template <int _Offset = 0, _ArchTraits _Traits = {}>
	[[__gnu__::__always_inline__]]
	constexpr _Bitmask<_S_size + _Offset>
	_M_to_uint() const
	{
	  constexpr int __nbits = _S_size;
	  static_assert(__nbits + _Offset <= numeric_limits<unsigned long long>::digits);
	  // before shifting
	  using _U0 = _Bitmask<__nbits>;
	  // potentially wider type needed for shift by _Offset
	  using _Ur = _Bitmask<__nbits + _Offset>;
	  if constexpr (_S_is_scalar || _S_use_bitmask)
	    {
	      auto __bits = _M_data;
	      if constexpr (_S_is_partial)
		__bits &= _S_implicit_mask;
	      return _Ur(__bits) << _Offset;
	    }
	  else
	    {
#if _GLIBCXX_X86
	      if (!__is_const_known(*this))
		{
		  _U0 __uint;
		  if constexpr (_Bytes != 2) // movmskb would duplicate each bit
		    __uint = _U0(__x86_movmsk(_M_data));
		  else if constexpr (_Bytes == 2 && _Traits._M_have_bmi2())
		    __uint = __bit_extract_even<__nbits>(__x86_movmsk(_M_data));
		  else if constexpr (_Bytes == 2)
		    return __similar_mask<char, __nbits, _Ap>(*this).template _M_to_uint<_Offset>();
		  else
		    static_assert(false);
		  // TODO: with AVX512 use __builtin_ia32_cvt[bwdq]2mask(128|256|512)
		  // TODO: Ask for compiler builtin to do the best of the above. This should also
		  // combine with a preceding vector-mask compare to produce a bit-mask compare (on
		  // AVX512)
		  if constexpr (_S_is_partial)
		    __uint &= (_U0(1) << _S_size) - 1;
		  return _Ur(__uint) << _Offset;
		}
#endif
	      using _IV = _VecType;
	      static_assert(destructible<_IV>);
	      const typename _IV::mask_type& __k = [&] [[__gnu__::__always_inline__]] () {
		if constexpr (is_same_v<typename _IV::mask_type, basic_mask>)
		  return *this;
		else
		  return typename _IV::mask_type(*this);
	      }();
	      constexpr int __n = _IV::size();
	      if constexpr (_Bytes * __CHAR_BIT__ >= __n) // '1 << __iota' cannot overflow
		{ // reduce(select(k, powers_of_2, 0))
		  constexpr _IV __pow2 = _IV(1) << __iota<_IV>;
		  return _Ur(_U0(__select_impl(__k, __pow2, _IV())
				   ._M_reduce(bit_or<>()))) << _Offset;
		}
	      else if constexpr (__n % __CHAR_BIT__ != 0)
		{ // recurse after splitting in two
		  constexpr int __n_lo = __n - __n % __CHAR_BIT__;
		  const auto [__lo, __hi] = chunk<__n_lo>(__k);
		  _Ur __bits = __hi.template _M_to_uint<_Offset + __n_lo>();
		  return __bits | __lo.template _M_to_uint<_Offset>();
		}
	      else
		{ // limit powers_of_2 to 1, 2, 4, ..., 128
		  constexpr _IV __pow2 = _IV(1) << (__iota<_IV> % _IV(__CHAR_BIT__));
		  _IV __x = __select_impl(__k, __pow2, _IV());
		  // partial reductions of 8 neighboring elements
		  __x |= _IV::_S_static_permute(__x, _SwapNeighbors<4>());
		  __x |= _IV::_S_static_permute(__x, _SwapNeighbors<2>());
		  __x |= _IV::_S_static_permute(__x, _SwapNeighbors<1>());
		  // permute partial reduction results to the front
		  __x = _IV::_S_static_permute(__x, [](int __i) {
			  return __i * 8 < __n ? __i * 8 : uninit_element;
			});
		  // extract front as scalar unsigned
		  _U0 __bits = __builtin_bit_cast(
				 __similar_vec<_U0, __n * _Bytes / sizeof(_U0), _Ap>, __x)[0];
		  // mask off unused bits
		  if constexpr (!__has_single_bit(unsigned(__nbits)))
		    __bits &= (_U0(1) << __nbits) - 1;
		  return _Ur(__bits) << _Offset;
		}
	    }
	}

      [[__gnu__::__always_inline__]]
      constexpr unsigned long long
      to_ullong() const
      { return _M_to_uint(); }

      // [simd.mask.binary] ---------------------------------------------------
      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator&&(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data & __y._M_data); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator||(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data | __y._M_data); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator&(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data & __y._M_data); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator|(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data | __y._M_data); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator^(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data ^ __y._M_data); }

      // [simd.mask.cassign] --------------------------------------------------
      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask&
      operator&=(basic_mask& __x, const basic_mask& __y) noexcept
      {
	__x._M_data &= __y._M_data;
	return __x;
      }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask&
      operator|=(basic_mask& __x, const basic_mask& __y) noexcept
      {
	__x._M_data |= __y._M_data;
	return __x;
      }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask&
      operator^=(basic_mask& __x, const basic_mask& __y) noexcept
      {
	__x._M_data ^= __y._M_data;
	return __x;
      }

      // [simd.mask.comparison] -----------------------------------------------
      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator==(const basic_mask& __x, const basic_mask& __y) noexcept
      { return !(__x ^ __y); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator!=(const basic_mask& __x, const basic_mask& __y) noexcept
      { return __x ^ __y; }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator>=(const basic_mask& __x, const basic_mask& __y) noexcept
      { return __x || !__y; }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator<=(const basic_mask& __x, const basic_mask& __y) noexcept
      { return !__x || __y; }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator>(const basic_mask& __x, const basic_mask& __y) noexcept
      { return __x && !__y; }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator<(const basic_mask& __x, const basic_mask& __y) noexcept
      { return !__x && __y; }

      // [simd.mask.cond] -----------------------------------------------------
      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      __select_impl(const basic_mask& __k, const basic_mask& __t, const basic_mask& __f) noexcept
      {
	if constexpr (!_S_use_bitmask)
	  {
#if _GLIBCXX_X86
	    // this works around bad code-gen when the compiler can't see that __k is a vector-mask.
	    // This pattern, is recognized to match the x86 blend instructions, which only consider
	    // the sign bit of the mask register. Also, without SSE4, if the compiler knows that __k
	    // is a vector-mask, then the '< 0' is elided.
	    return __k._M_data < 0 ? __t._M_data : __f._M_data;
#endif
	    return __k._M_data ? __t._M_data : __f._M_data;
	  }
	else
	  return (__k._M_data & __t._M_data) | (~__k._M_data & __f._M_data);
      }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      __select_impl(const basic_mask& __k, same_as<bool> auto __t, same_as<bool> auto __f) noexcept
      {
	if (__t == __f)
	  return basic_mask(__t);
	else
	  return __t ? __k : !__k;
      }

      template <__vectorizable _T0, same_as<_T0> _T1>
	requires (sizeof(_T0) == _Bytes)
	[[__gnu__::__always_inline__]]
	friend constexpr vec<_T0, _S_size>
	__select_impl(const basic_mask& __k, const _T0& __t, const _T1& __f) noexcept
	{
	  if constexpr (_S_is_scalar)
	    return __k._M_data ? __t : __f;
	  else
	    {
	      using _Vp = vec<_T0, _S_size>;
	      using _Mp = typename _Vp::mask_type;
	      return __select_impl(_Mp(__k), _Vp(__t), _Vp(__f));
	    }
	}

      // [simd.mask.reductions] implementation --------------------------------
      [[__gnu__::__always_inline__]]
      constexpr bool
      _M_all_of() const noexcept
      {
	if constexpr (_S_is_scalar)
	  return _M_data;
	else if constexpr (_S_use_bitmask)
	  {
	    if constexpr (_S_is_partial)
	      // PR120925 (partial kortest pattern not recognized)
	      return (_M_data & _S_implicit_mask) == _S_implicit_mask;
	    else
	      return _M_data == _S_implicit_mask;
	  }
#if _GLIBCXX_X86
	else if (!__is_const_known(_M_data))
	  return __x86_vecmask_all<_S_size>(_M_data);
#endif
	else
	  return _VecOps<_DataType, _S_size>::_S_all_of(_M_data);
      }

      [[__gnu__::__always_inline__]]
      constexpr bool
      _M_any_of() const noexcept
      {
	if constexpr (_S_is_scalar)
	  return _M_data;
	else if constexpr (_S_use_bitmask)
	  {
	    if constexpr (_S_is_partial)
	      // PR120925 (partial kortest pattern not recognized)
	      return (_M_data & _S_implicit_mask) != 0;
	    else
	      return _M_data != 0;
	  }
#if _GLIBCXX_X86
	else if (!__is_const_known(_M_data))
	  return __x86_vecmask_any<_S_size>(_M_data);
#endif
	else
	  return _VecOps<_DataType, _S_size>::_S_any_of(_M_data);
      }

      [[__gnu__::__always_inline__]]
      constexpr bool
      _M_none_of() const noexcept
      {
	if constexpr (_S_is_scalar)
	  return !_M_data;
	else if constexpr (_S_use_bitmask)
	  {
	    if constexpr (_S_is_partial)
	      // PR120925 (partial kortest pattern not recognized)
	      return (_M_data & _S_implicit_mask) == 0;
	    else
	      return _M_data == 0;
	  }
#if _GLIBCXX_X86
	else if (!__is_const_known(_M_data))
	  return __x86_vecmask_none<_S_size>(_M_data);
#endif
	else
	  return _VecOps<_DataType, _S_size>::_S_none_of(_M_data);
      }

      [[__gnu__::__always_inline__]]
      constexpr __simd_size_type
      _M_reduce_count() const noexcept
      {
	if constexpr (_S_is_scalar)
	  return int(_M_data);
	else if constexpr (_S_size <= numeric_limits<unsigned>::digits)
	  return __builtin_popcount(_M_to_uint());
	else
	  return __builtin_popcountll(to_ullong());
      }

      [[__gnu__::__always_inline__]]
      constexpr __simd_size_type
      _M_reduce_min_index() const
      {
	const auto __bits = _M_to_uint();
	__glibcxx_simd_precondition(__bits, "An empty mask does not have a min_index.");
	if constexpr (_S_size == 1)
	  return 0;
	else
	  return __countr_zero(__bits);
      }

      [[__gnu__::__always_inline__]]
      constexpr __simd_size_type
      _M_reduce_max_index() const
      {
	const auto __bits = _M_to_uint();
	__glibcxx_simd_precondition(__bits, "An empty mask does not have a max_index.");
	if constexpr (_S_size == 1)
	  return 0;
	else
	  return __highest_bit(__bits);
      }

      [[__gnu__::__always_inline__]]
      friend constexpr bool
      __is_const_known(const basic_mask& __x)
      { return __builtin_constant_p(__x._M_data); }
    };

  template <size_t _Bytes, __abi_tag _Ap>
    requires (_Ap::_S_nreg > 1)
    class basic_mask<_Bytes, _Ap>
    : public _MaskBase<_Bytes, _Ap>
    {
      using _Base = _MaskBase<_Bytes, _Ap>;

      using _VecType = _Base::_VecType;

      template <size_t, typename>
	friend class basic_mask;

      template <typename, typename>
	friend class basic_vec;

      static constexpr int _S_size = _Ap::_S_size;

      static constexpr int _N0 = __bit_ceil(unsigned(_S_size)) / 2;

      static constexpr int _N1 = _S_size - _N0;

      static constexpr int _Nreg0 = __bit_ceil(unsigned(_Ap::_S_nreg)) / 2;

      static constexpr int _Nreg1 = _Ap::_S_nreg - _Nreg0;

      // explicitly request _Nreg0 rather than use __abi_rebind. This way _Float16 can use half
      // of native registers (since they convert to full float32 registers).
      using _Abi0 = decltype(_Ap::template _S_resize<_N0, _Nreg0>());

      using _Abi1 = decltype(_Ap::template _S_resize<_N1, _Nreg1>());

      using _Mask0 = basic_mask<_Bytes, _Abi0>;

      // the implementation (and users) depend on elements being contiguous in memory
      static_assert(_Mask0::_S_padding_bytes == 0 && !_Mask0::_S_is_partial);

      using _Mask1 = basic_mask<_Bytes, _Abi1>;

      static constexpr bool _S_is_partial = _Mask1::_S_is_partial;

      // _Ap::_S_nreg determines how deep the recursion goes. E.g. basic_mask<4, _Abi<8, 4>> cannot
      // use basic_mask<4, _Abi<4, 1>> as _Mask0/1 types.
      static_assert(_Mask0::abi_type::_S_nreg + _Mask1::abi_type::_S_nreg == _Ap::_S_nreg);

      static constexpr bool _S_use_bitmask = _Mask0::_S_use_bitmask;

      static constexpr bool _S_is_scalar = _Mask0::_S_is_scalar;

      _Mask0 _M_data0;

      _Mask1 _M_data1;

      static constexpr bool _S_has_bool_member = _Mask1::_S_has_bool_member;

      // by construction _N0 >= _N1
      // => sizeof(_Mask0) >= sizeof(_Mask1)
      //    and __alignof__(_Mask0) >= __alignof__(_Mask1)
      static constexpr size_t _S_padding_bytes
	= (__alignof__(_Mask0) == __alignof__(_Mask1)
	     ? 0 : __alignof__(_Mask0) - (sizeof(_Mask1) % __alignof__(_Mask0)))
	    + _Mask1::_S_padding_bytes;

    public:
      using value_type = bool;

      using abi_type = _Ap;

      using iterator = _Base::iterator;

      using const_iterator = _Base::const_iterator;

      [[__gnu__::__always_inline__]]
      static constexpr basic_mask
      _S_init(const _Mask0& __x, const _Mask1& __y)
      {
	basic_mask __r;
	__r._M_data0 = __x;
	__r._M_data1 = __y;
	return __r;
      }

      [[__gnu__::__always_inline__]]
      static constexpr basic_mask
      _S_init(unsigned_integral auto __bits)
      { return basic_mask(__bits); }

      template <typename _U0, typename _U1>
	[[__gnu__::__always_inline__]]
	static constexpr basic_mask
	_S_init(const __trivial_pair<_U0, _U1>& __bits)
	{
	  if constexpr (is_unsigned_v<_U0>)
	    {
	      static_assert(is_unsigned_v<_U1>);
	      return _S_init(_Mask0(__bits._M_first), _Mask1(__bits._M_second));
	    }
	  else if constexpr (is_unsigned_v<_U1>)
	    return _S_init(_Mask0::_S_init(__bits._M_first), _Mask1(__bits._M_second));
	  else
	    return _S_init(_Mask0::_S_init(__bits._M_first), _Mask1::_S_init(__bits._M_second));
	}

      [[__gnu__::__always_inline__]]
      constexpr const _Mask0&
      _M_get_low() const
      { return _M_data0; }

      [[__gnu__::__always_inline__]]
      constexpr const _Mask1&
      _M_get_high() const
      { return _M_data1; }

      template <size_t _UBytes, typename _UAbi>
	[[__gnu__::__always_inline__]]
	static constexpr basic_mask
	_S_recursive_bit_cast(const basic_mask<_UBytes, _UAbi>& __x)
	{
	  using _Mp = basic_mask<_UBytes, _UAbi>;
	  if constexpr (_Mp::_S_has_bool_member || sizeof(basic_mask) > sizeof(__x)
			  || _Mp::_S_padding_bytes != 0)
	    return _S_init(__builtin_bit_cast(_Mask0, __x._M_data0),
			   _Mask1::_S_recursive_bit_cast(__x._M_data1));
	  else if constexpr (sizeof(basic_mask) == sizeof(__x))
	    return __builtin_bit_cast(basic_mask, __x);
	  else
	    { // e.g. on IvyBridge (different alignment => different sizeof)
	      struct _Tmp { alignas(_Mp) basic_mask _M_data; };
	      return __builtin_bit_cast(_Tmp, __x)._M_data;
	    }
	}

      [[__gnu__::__always_inline__]]
      constexpr auto
      _M_concat_data(bool __do_sanitize = _S_is_partial) const
      {
	if constexpr (_S_use_bitmask)
	  {
	    static_assert(_S_size <= numeric_limits<unsigned long long>::digits,
			  "cannot concat more than 64 bits");
	    using _Up = _Bitmask<_S_size>;
	    return _Up(_M_data0._M_concat_data() | (_Up(_M_data1._M_concat_data(__do_sanitize)) << _N0));
	  }
	else
	  {
	    auto __lo = _M_data0._M_concat_data();
	    auto __hi = __vec_zero_pad_to<sizeof(__lo)>(_M_data1._M_concat_data(__do_sanitize));
	    return __vec_concat(__lo, __hi);
	  }
      }

      template <_ArchTraits _Traits = {}>
	[[__gnu__::__always_inline__]]
	static constexpr basic_mask
	_S_partial_mask_of_n(int __n)
	{
#if __has_builtin(__builtin_ia32_bzhi_di)
	  if constexpr (_S_use_bitmask && _S_size <= 64 && _Traits._M_have_bmi2())
	    return basic_mask(__builtin_ia32_bzhi_di(~0ull >> (64 - _S_size), unsigned(__n)));
#endif
	  if constexpr (_N0 == 1)
	    {
	      static_assert(_S_size == 2); // => __n == 1
	      return _S_init(_Mask0(true), _Mask1(false));
	    }
	  else if (__n < _N0)
	    return _S_init(_Mask0::_S_partial_mask_of_n(__n), _Mask1(false));
	  else if (__n == _N0 || _N1 == 1)
	    return _S_init(_Mask0(true), _Mask1(false));
	  else if constexpr (_N1 != 1)
	    return _S_init(_Mask0(true), _Mask1::_S_partial_mask_of_n(__n - _N0));
	}

      [[__gnu__::__always_inline__]]
      constexpr basic_mask&
      _M_and_neighbors()
      {
	_M_data0._M_and_neighbors();
	_M_data1._M_and_neighbors();
	return *this;
      }

      [[__gnu__::__always_inline__]]
      constexpr basic_mask&
      _M_or_neighbors()
      {
	_M_data0._M_or_neighbors();
	_M_data1._M_or_neighbors();
	return *this;
      }

      template <typename _Mp>
	[[__gnu__::__always_inline__]]
	constexpr auto
	_M_chunk() const noexcept
	{
	  constexpr int __n = _S_size / _Mp::_S_size;
	  constexpr int __rem = _S_size % _Mp::_S_size;
	  constexpr auto [...__is] = _IotaArray<__n>;
	  if constexpr (__rem == 0)
	    return array<_Mp, __n>{__extract_simd_at<_Mp>(cw<_Mp::_S_size * __is>,
							    _M_data0, _M_data1)...};
	  else
	    {
	      using _Rest = resize_t<__rem, _Mp>;
	      return tuple(__extract_simd_at<_Mp>(cw<_Mp::_S_size * __is>, _M_data0, _M_data1)...,
			   __extract_simd_at<_Rest>(cw<_Mp::_S_size * __n>, _M_data0, _M_data1));
	    }
	}

      [[__gnu__::__always_inline__]]
      static constexpr basic_mask
      _S_concat(const basic_mask& __x0) noexcept
      { return __x0; }

      template <typename... _As>
	requires (sizeof...(_As) >= 2)
	[[__gnu__::__always_inline__]]
	static constexpr basic_mask
	_S_concat(const basic_mask<_Bytes, _As>&... __xs) noexcept
	{
	  static_assert(_S_size == (_As::_S_size + ...));
	  return _S_init(__extract_simd_at<_Mask0>(cw<0>, __xs...),
			 __extract_simd_at<_Mask1>(cw<_N0>, __xs...));
	}

      // [simd.mask.overview] default constructor -----------------------------
      basic_mask() = default;

      // [simd.mask.overview] conversion extensions ---------------------------
      // TODO: any?

      // [simd.mask.ctor] broadcast constructor -------------------------------
      [[__gnu__::__always_inline__]]
      constexpr explicit
      basic_mask(same_as<bool> auto __x) noexcept // LWG 4382.
	: _M_data0(__x), _M_data1(__x)
      {}

      // [simd.mask.ctor] conversion constructor ------------------------------
      template <size_t _UBytes, typename _UAbi>
	requires (_S_size == _UAbi::_S_size)
	[[__gnu__::__always_inline__]]
	constexpr explicit(__is_mask_conversion_explicit<_Ap, _UAbi>(_Bytes, _UBytes))
	basic_mask(const basic_mask<_UBytes, _UAbi>& __x) noexcept
	  : _M_data0([&] {
	      if constexpr (_UAbi::_S_nreg > 1)
		{
		    return __x._M_data0;
		}
	      else if constexpr (_N0 == 1)
		return _Mask0(__x[0]);
	      else
		return get<0>(chunk<_N0>(__x));
	    }()),
	    _M_data1([&] {
	      if constexpr (_UAbi::_S_nreg > 1)
		{
		    return __x._M_data1;
		}
	      else if constexpr (_N1 == 1)
		return _Mask1(__x[_N0]);
	      else
		return get<1>(chunk<_N0>(__x));
	    }())
	{}

      using _Base::_MaskBase;

      // [simd.mask.ctor] generator constructor -------------------------------
      template <__simd_generator_invokable<bool, _S_size> _Fp>
	[[__gnu__::__always_inline__]]
	constexpr explicit
	basic_mask(_Fp&& __gen)
	  : _M_data0(__gen), _M_data1([&] [[__gnu__::__always_inline__]] (auto __i) {
			       return __gen(__simd_size_c<__i + _N0>);
			     })
	{}

      // [simd.mask.ctor] bitset constructor ----------------------------------
      [[__gnu__::__always_inline__]]
      constexpr
      basic_mask(const same_as<bitset<_S_size>> auto& __b) noexcept // LWG 4382.
      : _M_data0(__bitset_split<_N0>(__b)._M_lo), _M_data1(__bitset_split<_N0>(__b)._M_hi)
      {}

      // [simd.mask.ctor] uint constructor ------------------------------------------
      template <unsigned_integral _Tp>
	requires (!same_as<_Tp, bool>) // LWG 4382.
	[[__gnu__::__always_inline__]]
	constexpr explicit
	basic_mask(_Tp __val) noexcept
	: _M_data0(static_cast<_Bitmask<_N0>>(__val)),
	  _M_data1(sizeof(_Tp) * __CHAR_BIT__ > _N0
		     ? static_cast<_Bitmask<_N1>>(__val >> _N0) : _Bitmask<_N1>())
	{}

      // [simd.mask.subscr] ---------------------------------------------------
      [[__gnu__::__always_inline__]]
      constexpr value_type
      operator[](__simd_size_type __i) const
      {
	__glibcxx_simd_precondition(__i >= 0 && __i < _S_size, "subscript is out of bounds");
	if (__is_const_known(__i))
	  return __i < _N0 ? _M_data0[__i] : _M_data1[__i - _N0];
	else if constexpr (_M_data1._S_has_bool_member)
	  // in some cases the last element can be 'bool' instead of bit-/vector-mask;
	  // e.g. mask<short, 17> is {mask<short, 16>, mask<short, 1>}, where the latter uses
	  // _ScalarAbi<1>, which is stored as 'bool'
	  return __i < _N0 ? _M_data0[__i] : _M_data1[__i - _N0];
	else if constexpr (abi_type::_S_is_bitmask)
	  {
	    using _AliasingByte [[__gnu__::__may_alias__]] = unsigned char;
	    return bool((reinterpret_cast<const _AliasingByte*>(this)
			   [__i / __CHAR_BIT__] >> (__i % __CHAR_BIT__)) & 1);
	  }
	else
	  {
	    using _AliasingInt [[__gnu__::__may_alias__]] = __integer_from<_Bytes>;
	    return reinterpret_cast<const _AliasingInt*>(this)[__i] != 0;
	  }
      }

      // [simd.mask.unary] ----------------------------------------------------
      [[__gnu__::__always_inline__]]
      constexpr basic_mask
      operator!() const noexcept
      { return _S_init(!_M_data0, !_M_data1); }

      [[__gnu__::__always_inline__]]
      constexpr _VecType
      operator+() const noexcept requires destructible<_VecType>
      { return _VecType::_S_concat(+_M_data0, +_M_data1); }

      constexpr _VecType
      operator+() const noexcept = delete;

      [[__gnu__::__always_inline__]]
      constexpr _VecType
      operator-() const noexcept requires destructible<_VecType>
      { return _VecType::_S_concat(-_M_data0, -_M_data1); }

      constexpr _VecType
      operator-() const noexcept = delete;

      [[__gnu__::__always_inline__]]
      constexpr _VecType
      operator~() const noexcept requires destructible<_VecType>
      { return _VecType::_S_concat(~_M_data0, ~_M_data1); }

      constexpr _VecType
      operator~() const noexcept = delete;

      // [simd.mask.conv] -----------------------------------------------------
      template <typename _Up, typename _UAbi>
	requires (_UAbi::_S_size == _S_size)
	[[__gnu__::__always_inline__]]
	constexpr explicit(sizeof(_Up) != _Bytes)
	operator basic_vec<_Up, _UAbi>() const noexcept
	{
	  using _Rp = basic_vec<_Up, _UAbi>;
	  return _Rp::_S_init(static_cast<_Rp::_DataType0>(_M_data0),
			      static_cast<_Rp::_DataType1>(_M_data1));
	}

      using _Base::operator basic_vec;

      // [simd.mask.namedconv] ------------------------------------------------
      [[__gnu__::__always_inline__]]
      constexpr bitset<_S_size>
      to_bitset() const noexcept
      {
	if constexpr (_S_size <= numeric_limits<unsigned long long>::digits)
	  return to_ullong();
	else
	  {
	    static_assert(_N0 % numeric_limits<unsigned long long>::digits == 0);
	    struct _Tmp
	    {
	      bitset<_N0> _M_lo;
	      bitset<_N1> _M_hi;
	    } __tmp = {_M_data0.to_bitset(), _M_data1.to_bitset()};
	    return __builtin_bit_cast(bitset<_S_size>, __tmp);
	  }
      }

      template <int _Offset = 0, _ArchTraits _Traits = {}>
	[[__gnu__::__always_inline__]]
	constexpr auto
	_M_to_uint() const
	{
	  constexpr int _N0x = _N0;
	  if constexpr (_N0x >= numeric_limits<unsigned long long>::digits)
	    {
	      static_assert(_Offset == 0);
	      return __trivial_pair {
		_M_data0.template _M_to_uint<0>(),
		_M_data1.template _M_to_uint<0>()
	      };
	    }
	  else
	    {
#if _GLIBCXX_X86
	      if constexpr (_Bytes == 2 && !_Traits._M_have_bmi2() && _Ap::_S_nreg == 2
			      && !_S_use_bitmask)
		return __similar_mask<char, _S_size, _Ap>(*this).template _M_to_uint<_Offset>();
#endif
	      auto __uint = _M_data1.template _M_to_uint<_N0x + _Offset>();
	      __uint |= _M_data0.template _M_to_uint<_Offset>();
	      return __uint;
	    }
	}

      [[__gnu__::__always_inline__]]
      constexpr unsigned long long
      to_ullong() const
      {
	if constexpr (_S_size <= numeric_limits<unsigned long long>::digits)
	  return _M_to_uint();
	else
	  {
	    __glibcxx_simd_precondition(_M_data1.to_ullong() == 0,
					"to_ullong called on mask with 'true' elements at indices"
					"higher than representable in a ullong");
	    return _M_data0.to_ullong();
	  }
      }

      // [simd.mask.binary]
      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator&&(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data0 && __y._M_data0, __x._M_data1 && __y._M_data1); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator||(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data0 || __y._M_data0, __x._M_data1 || __y._M_data1); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator&(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data0 & __y._M_data0, __x._M_data1 & __y._M_data1); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator|(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data0 | __y._M_data0, __x._M_data1 | __y._M_data1); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator^(const basic_mask& __x, const basic_mask& __y) noexcept
      { return _S_init(__x._M_data0 ^ __y._M_data0, __x._M_data1 ^ __y._M_data1); }

      // [simd.mask.cassign]
      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask&
      operator&=(basic_mask& __x, const basic_mask& __y) noexcept
      {
	__x._M_data0 &= __y._M_data0;
	__x._M_data1 &= __y._M_data1;
	return __x;
      }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask&
      operator|=(basic_mask& __x, const basic_mask& __y) noexcept
      {
	__x._M_data0 |= __y._M_data0;
	__x._M_data1 |= __y._M_data1;
	return __x;
      }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask&
      operator^=(basic_mask& __x, const basic_mask& __y) noexcept
      {
	__x._M_data0 ^= __y._M_data0;
	__x._M_data1 ^= __y._M_data1;
	return __x;
      }

      // [simd.mask.comparison] -----------------------------------------------
      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator==(const basic_mask& __x, const basic_mask& __y) noexcept
      { return !(__x ^ __y); }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator!=(const basic_mask& __x, const basic_mask& __y) noexcept
      { return __x ^ __y; }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator>=(const basic_mask& __x, const basic_mask& __y) noexcept
      { return __x || !__y; }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator<=(const basic_mask& __x, const basic_mask& __y) noexcept
      { return !__x || __y; }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator>(const basic_mask& __x, const basic_mask& __y) noexcept
      { return __x && !__y; }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      operator<(const basic_mask& __x, const basic_mask& __y) noexcept
      { return !__x && __y; }

      // [simd.mask.cond] -----------------------------------------------------
      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      __select_impl(const basic_mask& __k, const basic_mask& __t, const basic_mask& __f) noexcept
      {
	return _S_init(__select_impl(__k._M_data0, __t._M_data0, __f._M_data0),
		       __select_impl(__k._M_data1, __t._M_data1, __f._M_data1));
      }

      [[__gnu__::__always_inline__]]
      friend constexpr basic_mask
      __select_impl(const basic_mask& __k, same_as<bool> auto __t, same_as<bool> auto __f) noexcept
      {
	if (__t == __f)
	  return basic_mask(__t);
	else
	  return __t ? __k : !__k;
      }

      template <__vectorizable _T0, same_as<_T0> _T1>
	requires (sizeof(_T0) == _Bytes)
	[[__gnu__::__always_inline__]]
	friend constexpr vec<_T0, _S_size>
	__select_impl(const basic_mask& __k, const _T0& __t, const _T1& __f) noexcept
	{
	  using _Vp = vec<_T0, _S_size>;
	  if constexpr (!is_same_v<basic_mask, typename _Vp::mask_type>)
	    return __select_impl(static_cast<_Vp::mask_type>(__k), __t, __f);
	  else
	    return _Vp::_S_init(__select_impl(__k._M_data0, __t, __f),
				__select_impl(__k._M_data1, __t, __f));
	}

      template <_ArchTraits _Traits = {}>
	[[__gnu__::__always_inline__]]
	constexpr bool
	_M_all_of() const
	{
	  if constexpr (_N0 == _N1)
	    return (_M_data0 && _M_data1)._M_all_of();
	  else
	    return _M_data0._M_all_of() && _M_data1._M_all_of();
	}

      template <_ArchTraits _Traits = {}>
	[[__gnu__::__always_inline__]]
	constexpr bool
	_M_any_of() const
	{
	  if constexpr (_N0 == _N1)
	    return (_M_data0 || _M_data1)._M_any_of();
	  else
	    return _M_data0._M_any_of() || _M_data1._M_any_of();
	}

      template <_ArchTraits _Traits = {}>
	[[__gnu__::__always_inline__]]
	constexpr bool
	_M_none_of() const
	{
	  if constexpr (_N0 == _N1)
	    return (_M_data0 || _M_data1)._M_none_of();
	  else
	    return _M_data0._M_none_of() && _M_data1._M_none_of();
	}

      [[__gnu__::__always_inline__]]
      constexpr __simd_size_type
      _M_reduce_min_index() const
      {
	if constexpr (_S_size <= numeric_limits<unsigned long long>::digits)
	  {
	    const auto __bits = _M_to_uint();
	    __glibcxx_simd_precondition(__bits, "An empty mask does not have a min_index.");
	    if constexpr (_S_size == 1)
	      return 0;
	    else
	      return __countr_zero(_M_to_uint());
	  }
	else if (_M_data0._M_none_of())
	  return _M_data1._M_reduce_min_index() + _N0;
	else
	  return _M_data0._M_reduce_min_index();
      }

      [[__gnu__::__always_inline__]]
      constexpr __simd_size_type
      _M_reduce_max_index() const
      {
	if constexpr (_S_size <= numeric_limits<unsigned long long>::digits)
	  {
	    const auto __bits = _M_to_uint();
	    __glibcxx_simd_precondition(__bits, "An empty mask does not have a max_index.");
	    if constexpr (_S_size == 1)
	      return 0;
	    else
	      return __highest_bit(_M_to_uint());
	  }
	else if (_M_data1._M_none_of())
	  return _M_data0._M_reduce_max_index();
	else
	  return _M_data1._M_reduce_max_index() + _N0;
      }

      [[__gnu__::__always_inline__]]
      friend constexpr bool
      __is_const_known(const basic_mask& __x)
      { return __is_const_known(__x._M_data0) && __is_const_known(__x._M_data1); }
    };
} // namespace simd
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#pragma GCC diagnostic pop
#endif // C++26
#endif // _GLIBCXX_SIMD_MASK_H
