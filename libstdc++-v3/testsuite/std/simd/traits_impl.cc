// { dg-do compile { target c++26 } }
// { dg-require-effective-target x86 }

#define _GLIBCXX_SIMD_THROW_ON_BAD_VALUE 1

#include <bits/simd_details.h>
#include <bits/simd_flags.h>
#include <stdfloat>

namespace simd = std::simd;

#ifdef __STDCPP_FLOAT16_T__
using std::float16_t;
#endif
using std::float32_t;
using std::float64_t;

using namespace std::simd;
using std::__constexpr_wrapper_like;

template <int = 0>
void test()
{
  template for (auto t : {float(), double(),
#ifdef __STDCPP_FLOAT16_T__
			  float16_t(),
#endif
			  float32_t(), float64_t()})
    {
      using T = decltype(t);
      static_assert(__vectorizable<T>);
    }

  static_assert(!__vectorizable<const float>);
  static_assert(!__vectorizable<float&>);
#ifdef __STDCPP_BFLOAT16_T__
  static_assert(!__vectorizable<std::bfloat16_t>);
#endif

  template for (constexpr int N : {1, 2, 4, 8})
    {
      static_assert(std::signed_integral<__integer_from<N>>);
      static_assert(sizeof(__integer_from<N>) == N);
      static_assert(__vectorizable<__integer_from<N>>);
    }

  static_assert(__div_ceil(5, 3) == 2);

  static_assert(sizeof(_Bitmask<3>) == 1);
  static_assert(sizeof(_Bitmask<30>) == 4);

  static_assert(__scalar_abi_tag<_ScalarAbi<1>>);
  static_assert(__scalar_abi_tag<_ScalarAbi<2>>);
  static_assert(!__scalar_abi_tag<_Abi_t<1, 1>>);

  static_assert(__abi_tag<_ScalarAbi<1>>);
  static_assert(__abi_tag<_ScalarAbi<2>>);

  using AN = decltype(__native_abi<float>());
  using A1 = decltype(__native_abi<float>()._S_resize<1>());
  static_assert(A1::_S_size == 1);
  static_assert(A1::_S_nreg == 1);
  static_assert(A1::_S_variant == AN::_S_variant);
  static_assert(__scalar_abi_tag<A1> == __scalar_abi_tag<AN>);
  static_assert(std::is_same_v<decltype(__abi_rebind<float, AN::_S_size, A1>()), AN>);
  if constexpr (AN::_S_size >= 2) // the target has SIMD support for float
    {
      {
	using A2 = decltype(__abi_rebind<float, 2, AN>());
	static_assert(A2::_S_size == 2);
	static_assert(A2::_S_nreg == 1);
	static_assert(A2::_S_variant == AN::_S_variant);
	using A2x = decltype(__abi_rebind<float, 2, decltype(__abi_rebind<float, 1, A2>())>());
	static_assert(std::is_same_v<A2, A2x>);
      }
      using A4 = decltype(__abi_rebind<float, 4, AN>());
      static_assert(A4::_S_size == 4);
    }

  static_assert(__streq_to_1("1"));
  static_assert(!__streq_to_1(""));
  static_assert(!__streq_to_1(nullptr));
  static_assert(!__streq_to_1("0"));
  static_assert(!__streq_to_1("1 "));

  static_assert(__static_sized_range<int[4]>);
  static_assert(__static_sized_range<int[4], 4>);
  static_assert(__static_sized_range<std::array<int, 4>, 4>);

  static_assert( __value_preserving_convertible_to<int, double>);
  static_assert(!__value_preserving_convertible_to<int, float>);
  static_assert( __value_preserving_convertible_to<float, double>);
  static_assert(!__value_preserving_convertible_to<double, float>);

#ifdef __STDCPP_FLOAT16_T__
  static_assert(__explicitly_convertible_to<float, float16_t>);
  static_assert(__explicitly_convertible_to<long, float16_t>);
#endif

  static_assert(__constexpr_wrapper_like<std::constant_wrapper<2>>);
  static_assert(__constexpr_wrapper_like<std::integral_constant<int, 1>>);

  static_assert(!__broadcast_constructible<int, float>);
  static_assert(!__broadcast_constructible<int&, float>);
  static_assert(!__broadcast_constructible<int&&, float>);
  static_assert(!__broadcast_constructible<const int&, float>);
  static_assert(!__broadcast_constructible<const int, float>);

  static_assert(__broadcast_constructible<decltype(std::cw<2>), float>);
#ifdef __STDCPP_FLOAT16_T__
  static_assert(__broadcast_constructible<decltype(std::cw<0.f>), std::float16_t>);
#endif


  static_assert(__higher_rank_than<long, int>);
  static_assert(__higher_rank_than<long long, long>);
  static_assert(__higher_rank_than<int, short>);
  static_assert(__higher_rank_than<short, char>);

  static_assert(!__higher_rank_than<char, signed char>);
  static_assert(!__higher_rank_than<signed char, char>);
  static_assert(!__higher_rank_than<char, unsigned char>);
  static_assert(!__higher_rank_than<unsigned char, char>);

  static_assert(__higher_rank_than<unsigned int, short>);
  static_assert(__higher_rank_than<unsigned long, int>);
  static_assert(__higher_rank_than<unsigned long long, long>);

#ifdef __STDCPP_FLOAT16_T__
  static_assert(__higher_rank_than<float, float16_t>);
#endif
  static_assert(__higher_rank_than<float32_t, float>);
  static_assert(__higher_rank_than<double, float32_t>);
  static_assert(__higher_rank_than<double, float>);
  static_assert(__higher_rank_than<float64_t, float32_t>);
  static_assert(__higher_rank_than<float64_t, float>);
  static_assert(__higher_rank_than<float64_t, double>);

  static_assert(__loadstore_convertible_to<float, double>);
  static_assert(__loadstore_convertible_to<int, double>);
  static_assert(!__loadstore_convertible_to<int, float>);
  static_assert(!__loadstore_convertible_to<int, float, __aligned_flag>);
  static_assert(__loadstore_convertible_to<int, float, __convert_flag>);
  static_assert(__loadstore_convertible_to<int, float, __aligned_flag, __convert_flag>);

  static_assert(__mask_element_size<basic_mask<4>> == 4);

  static_assert(__highest_bit(0b1000u) == 3);
  static_assert(__highest_bit(0b10000001000ull) == 10);
}

template void test<>();

consteval bool
throws(auto f)
{
  try { f(); }
  catch (...) { return true; }
  return false;
}

static_assert(!throws([] { __value_preserving_cast<float>(1); }));
static_assert(!throws([] { __value_preserving_cast<float>(1.5); }));
static_assert(throws([] { __value_preserving_cast<float>(0x5EAF00D); }));
static_assert(throws([] { __value_preserving_cast<unsigned>(-1); }));
static_assert(!throws([] { __value_preserving_cast<unsigned short>(0xffff); }));
static_assert(throws([] { __value_preserving_cast<unsigned short>(0x10000); }));

static_assert(__converts_trivially<int, unsigned>);
#if __SIZEOF_LONG__ == __SIZEOF_LONG_LONG__
static_assert(__converts_trivially<long long, long>);
#elif __SIZEOF_INT__ == __SIZEOF_LONG__
static_assert(__converts_trivially<int, long>);
#endif
static_assert(__converts_trivially<float, float32_t>);

static_assert([] {
  bool to_find[10] = {0, 1, 1, 1, 0, 1, 0, 0, 1};
  __bit_foreach(0b100101110u, [&](int i) {
    if (!to_find[i]) throw false;
    to_find[i] = false;
  });
  for (bool b : to_find)
    if (b)
      return false;
  return true;
}());

// flags ////////////////////////
static_assert(std::is_same_v<decltype(flag_default | flag_default), flags<>>);
static_assert(std::is_same_v<decltype(flag_convert | flag_default), flags<__convert_flag>>);
static_assert(std::is_same_v<decltype(flag_convert | flag_convert), flags<__convert_flag>>);
static_assert(std::is_same_v<decltype(flag_aligned | flag_convert),
			     flags<__aligned_flag, __convert_flag>>);
static_assert(std::is_same_v<decltype(flag_aligned | flag_convert | flag_aligned),
			     flags<__aligned_flag, __convert_flag>>);
static_assert(std::is_same_v<decltype(flag_aligned | (flag_convert | flag_aligned)),
			     flags<__aligned_flag, __convert_flag>>);

static_assert(!flag_default._S_test(flag_convert));
static_assert(flag_convert._S_test(flag_convert));
static_assert(!flag_convert._S_test(flag_aligned));
static_assert((flag_overaligned<32> | flag_convert | flag_aligned)._S_test(flag_convert));
