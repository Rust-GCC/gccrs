// Test framework for <simd> -*- C++ -*-

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

#ifndef SIMD_TEST_SETUP_H
#define SIMD_TEST_SETUP_H

#include <bits/simd_details.h>
#include <string_view>

namespace test
{
  struct precondition_failure
  {
    std::string_view file;
    int line;
    std::string_view expr;
    std::string_view msg;
  };

#undef __glibcxx_simd_precondition

#define __glibcxx_simd_precondition(expr, msg, ...) \
  do {                                              \
    if (__builtin_expect(!bool(expr), false))       \
      throw test::precondition_failure{__FILE__, __LINE__, #expr, msg}; \
  } while(false)
}

#undef _GLIBCXX_SIMD_NOEXCEPT
#define _GLIBCXX_SIMD_NOEXCEPT noexcept(false)

#include <simd>

#include <source_location>
#include <iostream>
#include <concepts>
#include <cfenv>
#include <vector>
#include <cstdint>
#include <climits>

// global objects
static std::vector<void(*)()> test_functions = {};

static std::int64_t passed_tests = 0;

static std::int64_t failed_tests = 0;

static std::string_view test_name = "unknown";

// ------------------------------------------------

namespace simd = std::simd;

template <typename T>
  struct is_character_type
  : std::bool_constant<false>
  {};

template <typename T>
  inline constexpr bool is_character_type_v = is_character_type<T>::value;

template <typename T>
  struct is_character_type<const T>
  : is_character_type<T>
  {};

template <typename T>
  struct is_character_type<T&>
  : is_character_type<T>
  {};

template <> struct is_character_type<char> : std::bool_constant<true> {};
template <> struct is_character_type<wchar_t> : std::bool_constant<true> {};
template <> struct is_character_type<char8_t> : std::bool_constant<true> {};
template <> struct is_character_type<char16_t> : std::bool_constant<true> {};
template <> struct is_character_type<char32_t> : std::bool_constant<true> {};

std::ostream& operator<<(std::ostream& s, std::byte b)
{ return s << std::hex << static_cast<unsigned>(b) << std::dec; }

template <typename T, typename Abi>
std::ostream& operator<<(std::ostream& s, std::simd::basic_vec<T, Abi> const& v)
{
  if constexpr (std::is_arithmetic_v<T>)
    {
      using U = std::conditional_t<
		  sizeof(T) == 1, int, std::conditional_t<
					 is_character_type_v<T>,
					 std::simd::_UInt<sizeof(T)>, T>>;
      s << '[' << U(v[0]);
      for (int i = 1; i < v.size(); ++i)
	s << ", " << U(v[i]);
    }
  else
    {
      s << '[' << v[0];
      for (int i = 1; i < v.size(); ++i)
	s << ", " << v[i];
    }
  return s << ']';
}

template <std::size_t B, typename Abi>
std::ostream& operator<<(std::ostream& s, std::simd::basic_mask<B, Abi> const& v)
{
  s << '<';
  for (int i = 0; i < v.size(); ++i)
    s << int(v[i]);
  return s << '>';
}

template <std::simd::__vec_builtin V>
  std::ostream& operator<<(std::ostream& s, V v)
  { return s << std::simd::vec<std::simd::__vec_value_type<V>, std::simd::__width_of<V>>(v); }

template <typename T, typename U>
  std::ostream& operator<<(std::ostream& s, const std::pair<T, U>& x)
  { return s << '{' << x.first << ", " << x.second << '}'; }

template <typename T>
  concept is_string_type
    = is_character_type_v<std::ranges::range_value_t<T>>
	&& std::is_convertible_v<T, std::basic_string_view<std::ranges::range_value_t<T>>>;

template <std::ranges::range R>
  requires (!is_string_type<R>)
  std::ostream& operator<<(std::ostream& s, R&& x)
  {
    s << '[';
    auto it = std::ranges::begin(x);
    if (it != std::ranges::end(x))
      {
	s << *it;
	while (++it != std::ranges::end(x))
	  s << ',' << *it;
      }
    return s << ']';
  }

struct additional_info
{
  const bool failed = false;

  additional_info
  operator()(auto const& value0, auto const&... more)
  {
    if (failed)
      [&] {
	std::cout << "  " << value0;
	((std::cout << ' ' << more), ...);
	std::cout << std::endl;
      }();
    return *this;
  }
};

struct log_novalue {};

template <typename T>
  struct unwrap_value_types
  { using type = T; };

template <typename T>
  requires requires { typename T::value_type; }
  struct unwrap_value_types<T>
  { using type = typename unwrap_value_types<typename T::value_type>::type; };

template <typename T>
  using value_type_t = typename unwrap_value_types<std::remove_cvref_t<T>>::type;

template <typename T>
  struct as_unsigned;

template <typename T>
  using as_unsigned_t = typename as_unsigned<T>::type;

template <typename T>
  requires (sizeof(T) == sizeof(unsigned char))
  struct as_unsigned<T>
  { using type = unsigned char; };

template <typename T>
  requires (sizeof(T) == sizeof(unsigned short))
  struct as_unsigned<T>
  { using type = unsigned short; };

template <typename T>
  requires (sizeof(T) == sizeof(unsigned int))
  struct as_unsigned<T>
  { using type = unsigned int; };

template <typename T>
  requires (sizeof(T) == sizeof(unsigned long long))
  struct as_unsigned<T>
  { using type = unsigned long long; };

template <typename T, typename Abi>
  struct as_unsigned<std::simd::basic_vec<T, Abi>>
  { using type = std::simd::rebind_t<as_unsigned_t<T>, std::simd::basic_vec<T, Abi>>; };

template <typename T0, typename T1>
  constexpr T0
  ulp_distance_signed(T0 val0, const T1& ref1)
  {
    if constexpr (std::is_floating_point_v<T1>)
      return ulp_distance_signed(val0, std::simd::rebind_t<T1, T0>(ref1));
    else if constexpr (std::is_floating_point_v<value_type_t<T0>>)
      {
	int fp_exceptions = 0;
	if !consteval
	  {
	    fp_exceptions = std::fetestexcept(FE_ALL_EXCEPT);
	  }
	using std::isnan;
	using std::abs;
	using T = value_type_t<T0>;
	using L = std::numeric_limits<T>;
	constexpr T0 signexp_mask = -L::infinity();
	T0 ref0(ref1);
	T1 val1(val0);
	const auto subnormal = fabs(ref1) < L::min();
	using I = as_unsigned_t<T1>;
	const T1 eps1 = select(subnormal, L::denorm_min(),
			       L::epsilon() * std::bit_cast<T0>(
						std::bit_cast<I>(ref1)
						  & std::bit_cast<I>(signexp_mask)));
	const T0 ulp = select(val0 == ref0 || (isnan(val0) && isnan(ref0)),
			      T0(), T0((ref1 - val1) / eps1));
	if !consteval
	  {
	    std::feclearexcept(FE_ALL_EXCEPT ^ fp_exceptions);
	  }
	return ulp;
      }
    else
      return ref1 - val0;
  }

template <typename T0, typename T1>
  constexpr T0
  ulp_distance(const T0& val, const T1& ref)
  {
    auto ulp = ulp_distance_signed(val, ref);
    using T = value_type_t<decltype(ulp)>;
    if constexpr (std::is_unsigned_v<T>)
      return ulp;
    else
      {
	using std::abs;
	return fabs(ulp);
      }
  }

template <typename T>
  constexpr bool
  bit_equal(const T& a, const T& b)
  {
    using std::simd::_UInt;
    if constexpr (sizeof(T) <= sizeof(0ull))
      return std::bit_cast<_UInt<sizeof(T)>>(a) == std::bit_cast<_UInt<sizeof(T)>>(b);
    else if constexpr (std::simd::__simd_vec_or_mask_type<T>)
      {
	using TT = typename T::value_type;
	if constexpr (std::is_integral_v<TT>)
	  return all_of(a == b);
	else
	  {
	    constexpr size_t uint_size = std::min(size_t(8), sizeof(TT));
	    struct B
	    {
	      alignas(T) simd::rebind_t<_UInt<uint_size>,
					simd::resize_t<T::size() * sizeof(TT) / uint_size, T>> data;
	    };
	    if constexpr (sizeof(B) == sizeof(a))
	      return all_of(std::bit_cast<B>(a).data == std::bit_cast<B>(b).data);
	    else
	      {
		auto [a0, a1] = chunk<std::bit_ceil(unsigned(T::size())) / 2>(a);
		auto [b0, b1] = chunk<std::bit_ceil(unsigned(T::size())) / 2>(b);
		return bit_equal(a0, b0) && bit_equal(a1, b1);
	      }
	  }
      }
    else
      static_assert(false);
  }

// treat as equal if either:
// - operator== yields true
// - or for floats, a and b are NaNs
template <typename V>
  constexpr bool
  equal_with_nan_and_inf_fixup(const V& a, const V& b)
  {
    auto eq = a == b;
    if (std::simd::all_of(eq))
      return true;
    else if constexpr (std::simd::__simd_vec_type<V>)
      {
	using M = typename V::mask_type;
	using T = typename V::value_type;
	if constexpr (std::is_floating_point_v<T>)
	  { // fix up nan == nan results
	    eq |= a._M_isnan() && b._M_isnan();
	  }
	else
	  return false;
	return std::simd::all_of(eq);
      }
    else if constexpr (std::is_floating_point_v<V>)
      return std::isnan(a) && std::isnan(b);
    else
      return false;
  }

struct constexpr_verifier
{
  struct ignore_the_rest
  {
    constexpr ignore_the_rest
    operator()(auto const&, auto const&...)
    { return *this; }
  };

  bool okay = true;

  constexpr ignore_the_rest
  verify_precondition_failure(std::string_view expected_msg, auto&& f) &
  {
    try
      {
	f();
	okay = false;
      }
    catch (const test::precondition_failure& failure)
      {
	okay = okay && failure.msg == expected_msg;
      }
    catch (...)
      {
	okay = false;
      }
    return {};
  }

  constexpr ignore_the_rest
  verify(const auto& k) &
  {
    okay = okay && std::simd::all_of(k);
    return {};
  }

  constexpr ignore_the_rest
  verify_equal(const auto& v, const auto& ref) &
  {
    using V = decltype(std::simd::select(v == ref, v, ref));
    okay = okay && equal_with_nan_and_inf_fixup<V>(v, ref);
    return {};
  }

  constexpr ignore_the_rest
  verify_bit_equal(const auto& v, const auto& ref) &
  {
    using V = decltype(std::simd::select(v == ref, v, ref));
    okay = okay && bit_equal<V>(v, ref);
    return {};
  }

  template <typename T, typename U>
    constexpr ignore_the_rest
    verify_equal(const std::pair<T, U>& x, const std::pair<T, U>& y) &
    {
      verify_equal(x.first, y.first);
      verify_equal(x.second, y.second);
      return {};
    }

  constexpr ignore_the_rest
  verify_not_equal(const auto& v, const auto& ref) &
  {
    okay = okay && std::simd::all_of(v != ref);
    return {};
  }

  constexpr ignore_the_rest
  verify_equal_to_ulp(const auto& x, const auto& y, float allowed_distance) &
  {
    okay = okay && std::simd::all_of(ulp_distance(x, y) <= allowed_distance);
    return {};
  }

  constexpr_verifier() = default;

  constexpr_verifier(const constexpr_verifier&) = delete;

  constexpr_verifier(constexpr_verifier&&) = delete;
};

template <int... is>
  [[nodiscard]]
  consteval bool
  constexpr_test(auto&& fun, auto&&... args)
  {
    constexpr_verifier t;
    try
      {
	fun.template operator()<is...>(t, args...);
      }
    catch(const test::precondition_failure& fail)
      {
	return false;
      }
    return t.okay;
  }

template <typename T>
  T
  make_value_unknown(const T& x)
  { return *std::start_lifetime_as<T>(&x); }

template <typename T>
  concept pair_specialization
    = std::same_as<std::remove_cvref_t<T>, std::pair<typename std::remove_cvref_t<T>::first_type,
						     typename std::remove_cvref_t<T>::second_type>>;

struct runtime_verifier
{
  const std::string_view test_kind;

  template <typename X, typename Y>
    additional_info
    log_failure(const X& x, const Y& y, std::source_location loc, std::string_view s)
    {
      ++failed_tests;
      std::cout << loc.file_name() << ':' << loc.line() << ':' << loc.column() << ": in "
		<< test_kind << " test of '" << test_name
		<< "' " << s << " failed";
      if constexpr (!std::is_same_v<X, log_novalue>)
	{
	  std::cout << ":\n   result: " << std::boolalpha;
	  if constexpr (is_character_type_v<X>)
	    std::cout << int(x);
	  else
	    std::cout << x;
	  if constexpr (!std::is_same_v<decltype(y), const log_novalue&>)
	    {
	      std::cout << "\n expected: ";
	      if constexpr (is_character_type_v<Y>)
		std::cout << int(y);
	      else
		std::cout << y;
	    }
	}
      std::cout << std::endl;
      return additional_info {true};
    }

  [[gnu::always_inline]]
  additional_info
  verify_precondition_failure(std::string_view expected_msg, auto&& f,
			      std::source_location loc = std::source_location::current()) &
  {
    try
      {
	f();
	++failed_tests;
	return log_failure(log_novalue(), log_novalue(), loc, "precondition failure not detected");
      }
    catch (const test::precondition_failure& failure)
      {
	if (failure.msg != expected_msg)
	  {
	    ++failed_tests;
	    return log_failure(failure.msg, expected_msg, loc, "unexpected exception");
	  }
	else
	  {
	    ++passed_tests;
	    return {};
	  }
      }
    catch (...)
      {
	++failed_tests;
	return log_failure(log_novalue(), log_novalue(), loc, "unexpected exception");
      }
  }

  [[gnu::always_inline]]
  additional_info
  verify(auto&& k, std::source_location loc = std::source_location::current())
  {
    if (std::simd::all_of(k))
      {
	++passed_tests;
	return {};
      }
    else
      return log_failure(log_novalue(), log_novalue(), loc, "verify");
  }

  [[gnu::always_inline]]
  additional_info
  verify_equal(auto&& x, auto&& y,
	       std::source_location loc = std::source_location::current())
  {
    bool ok;
    if constexpr (pair_specialization<decltype(x)> && pair_specialization<decltype(y)>)
      ok = std::simd::all_of(x.first == y.first) && std::simd::all_of(x.second == y.second);
    else
      ok = equal_with_nan_and_inf_fixup<decltype(std::simd::select(x == y, x, y))>(x, y);
    if (ok)
      {
	++passed_tests;
	return {};
      }
    else
      return log_failure(x, y, loc, "verify_equal");
  }

  [[gnu::always_inline]]
  additional_info
  verify_bit_equal(auto&& x, auto&& y,
		   std::source_location loc = std::source_location::current())
  {
    using V = decltype(std::simd::select(x == y, x, y));
    if (bit_equal<V>(x, y))
      {
	++passed_tests;
	return {};
      }
    else
      return log_failure(x, y, loc, "verify_bit_equal");
  }

  [[gnu::always_inline]]
  additional_info
  verify_not_equal(auto&& x, auto&& y,
		   std::source_location loc = std::source_location::current())
  {
    if (std::simd::all_of(x != y))
      {
	++passed_tests;
	return {};
      }
    else
      return log_failure(x, y, loc, "verify_not_equal");
  }

  // ulp_distance_signed can raise FP exceptions and thus must be conditionally executed
  [[gnu::always_inline]]
  additional_info
  verify_equal_to_ulp(auto&& x, auto&& y, float allowed_distance,
		      std::source_location loc = std::source_location::current())
  {
    const bool success = std::simd::all_of(ulp_distance(x, y) <= allowed_distance);
    if (success)
      {
	++passed_tests;
	return {};
      }
    else
      return log_failure(x, y, loc, "verify_equal_to_ulp")
	       ("distance:", ulp_distance_signed(x, y),
		"\n allowed:", allowed_distance);
  }
};

template <int... is>
  [[gnu::noinline, gnu::noipa]]
  void
  runtime_test(auto&& fun, auto&&... args)
  {
    runtime_verifier t {"runtime"};
    fun.template operator()<is...>(t, make_value_unknown(args)...);
  }

template <typename T>
  concept constant_value = requires {
    typename std::integral_constant<std::remove_cvref_t<decltype(T::value)>, T::value>;
  };

template <typename T>
  [[gnu::always_inline]] inline bool
  is_const_known(const T& x)
  { return constant_value<T> || __builtin_constant_p(x); }

template <typename T, typename Abi>
  [[gnu::always_inline]] inline bool
  is_const_known(const std::simd::basic_vec<T, Abi>& x)
  { return __is_const_known(x); }

template <std::size_t B, typename Abi>
  [[gnu::always_inline]] inline bool
  is_const_known(const std::simd::basic_mask<B, Abi>& x)
  { return __is_const_known(x); }

template <std::ranges::sized_range R>
  [[gnu::always_inline]] inline bool
  is_const_known(const R& arr)
  {
    constexpr std::size_t N = std::ranges::size(arr);
    constexpr auto [...is] = std::_IotaArray<N>;
    return (is_const_known(arr[is]) && ...);
  }

template <int... is>
  [[gnu::always_inline, gnu::flatten]]
  inline void
  constprop_test(auto&& fun, auto... args)
  {
    runtime_verifier t{"constprop"};
#ifndef __clang__
    t.verify((is_const_known(args) && ...))("=> Some argument(s) failed to constant-propagate.");
#endif
    fun.template operator()<is...>(t, args...);
  }

/**
 * The value of the largest element in test_iota<V, Init>.
 */
template <typename V, int Init = 0, int Max = V::size() + Init - 1>
  constexpr value_type_t<V> test_iota_max
    = sizeof(value_type_t<V>) < sizeof(int)
	? std::min(int(std::numeric_limits<value_type_t<V>>::max()),
		   Max < 0 ? std::min(V::size() + Init - 1,
				      int(std::numeric_limits<value_type_t<V>>::max()) + Max)
			   : Max)
	: V::size() + Init - 1;

template <typename T, typename Abi, int Init, int Max>
  requires std::is_enum_v<T>
  constexpr T test_iota_max<simd::basic_vec<T, Abi>, Init, Max>
    = static_cast<T>(test_iota_max<simd::basic_vec<std::underlying_type_t<T>, Abi>, Init, Max>);

/**
 * Starts iota sequence at Init.
 *
 * With `Max == 0`: Wrap-around on overflow
 * With `Max < 0`: Subtract from numeric_limits::max (to leave room for arithmetic ops)
 * Otherwise: [Init..Max, Init..Max, ...] (inclusive)
 *
 * Use simd::__iota if a non-monotonic sequence is a bug.
 */
template <typename V, int Init = 0, int MaxArg = int(test_iota_max<V, Init>)>
  constexpr V test_iota = V([](int i) {
	      constexpr int Max = MaxArg < 0 ? int(test_iota_max<V, Init, MaxArg>) : MaxArg;
	      static_assert(Max == 0 || Max > Init || V::size() == 1);
	      i += Init;
	      if constexpr (Max > Init)
		{
		  while (i > Max)
		    i -= Max - Init + 1;
		}
	      using T = value_type_t<V>;
		return static_cast<T>(i);
	    });

/**
 * A data-parallel object initialized with {values..., values..., ...}
 */
template <typename V, auto... values>
  constexpr V init_vec = [] {
    using T = typename V::value_type;
    constexpr std::array<T, sizeof...(values)> arr = {T(values)...};
    return V([&](size_t i) { return arr[i % arr.size()]; });
  }();

template <typename V>
  struct Tests;

template <typename T>
  concept array_specialization
    = std::same_as<T, std::array<typename T::value_type, std::tuple_size_v<T>>>;

template <typename Args = void, typename Fun = void>
  struct add_test
  {
    alignas(std::bit_floor(sizeof(Args))) Args args;
    Fun fun;
  };

struct dummy_test
{
  static constexpr std::array<int, 0> args = {};
  static constexpr auto fun = [](auto&, auto...) {};
};

template <auto test_ref, int... is, std::size_t... arg_idx>
  void
  invoke_test_impl(std::index_sequence<arg_idx...>)
  {
    constexpr auto fun = test_ref->fun;
    [[maybe_unused]] constexpr auto args = test_ref->args;
#ifdef EXPENSIVE_TESTS
    constprop_test<is...>(fun, std::get<arg_idx>(args)...);
    constexpr bool passed = constexpr_test<is...>(fun, std::get<arg_idx>(args)...);
    if (passed)
      ++passed_tests;
    else
      {
	++failed_tests;
	std::cout << "=> constexpr test of '" << test_name << "' failed.\n";
      }
#endif
    runtime_test<is...>(fun, std::get<arg_idx>(args)...);
  }

template <auto test_ref, int... is>
  void
  invoke_test(std::string_view name)
  {
    test_name = name;
    constexpr auto args = test_ref->args;
    using A = std::remove_const_t<decltype(args)>;
    if constexpr (array_specialization<A>)
      { // call for each element
	template for (constexpr std::size_t I : std::_IotaArray<args.size()>)
	  {
	    std::string tmp_name = std::string(name) + '|' + std::to_string(I);
	    test_name = tmp_name;
	    ((std::cout << "Testing '" << test_name) << ... << (' ' + std::to_string(is)))
	      << ' ' << args[I] << "'\n";
	    invoke_test_impl<test_ref, is...>(std::index_sequence<I>());
	  }
      }
    else
      {
	((std::cout << "Testing '" << test_name) << ... << (' ' + std::to_string(is))) << "'\n";
	invoke_test_impl<test_ref, is...>(std::make_index_sequence<std::tuple_size_v<A>>());
      }
  }

#define ADD_TEST(name, ...)                                                                        \
    template <int>                                                                                 \
      static constexpr auto name##_tmpl = dummy_test {};                                           \
												   \
    const int init_##name = [] {                                                                   \
      test_functions.push_back([] { invoke_test<&name##_tmpl<0>>(#name); });                       \
      return 0;                                                                                    \
    }();                                                                                           \
												   \
    template <int Tmp>                                                                             \
      requires (Tmp == 0) __VA_OPT__(&& (__VA_ARGS__))                                             \
      static constexpr auto name##_tmpl<Tmp> = add_test

#define ADD_TEST_N(name, N, ...)                                                                   \
    template <int>                                                                                 \
      static constexpr auto name##_tmpl = dummy_test {};                                           \
												   \
    static void                                                                                    \
    name()                                                                                         \
    {                                                                                              \
      template for (constexpr int i : std::_IotaArray<N, int>)                                     \
	invoke_test<&name##_tmpl<0>, i>(#name);                                                    \
    }                                                                                              \
												   \
    const int init_##name = [] {                                                                   \
      test_functions.push_back(name);                                                              \
      return 0;                                                                                    \
    }();                                                                                           \
												   \
    template <int Tmp>                                                                             \
      requires (Tmp == 0) __VA_OPT__(&& (__VA_ARGS__))                                             \
      static constexpr auto name##_tmpl<Tmp> = add_test

void create_tests();

int main()
{
  create_tests();
  try
    {
      for (auto f : test_functions)
	f();
    }
  catch(const test::precondition_failure& fail)
    {
      std::cout << fail.file << ':' << fail.line << ": Error: precondition '" << fail.expr
		<< "' does not hold: " << fail.msg << '\n';
      return EXIT_FAILURE;
    }
  std::cout << "Passed tests: " << passed_tests << "\nFailed tests: " << failed_tests << '\n';
  return failed_tests != 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}

#endif  // SIMD_TEST_SETUP_H
