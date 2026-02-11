// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }

#include "test_setup.h"

template <typename T, std::size_t N, std::size_t Alignment>
  class alignas(Alignment) aligned_array
    : public std::array<T, N>
  {};

inline constexpr std::multiplies<> mul;
inline constexpr std::bit_and<> bit_and;
inline constexpr std::bit_or<> bit_or;
inline constexpr std::bit_xor<> bit_xor;

inline constexpr auto my_add = [](auto a, auto b) { return a + b; };

template <typename V>
  struct Tests
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    static_assert(simd::alignment_v<V> <= 256);

    static consteval V
    poisoned(T x)
    {
      if constexpr (sizeof(V) == sizeof(T) * V::size())
	return V(x);
      else
	{
	  using P = simd::resize_t<sizeof(V) / sizeof(T), V>;
	  static_assert(P::size() > V::size());
	  constexpr auto [...is] = std::_IotaArray<P::size()>;
	  const T arr[P::size()] = {(is < V::size() ? x : T(7))...};
	  return std::bit_cast<V>(P(arr));
	}
    }

    ADD_TEST(Sum) {
      std::tuple {poisoned(0), poisoned(1)},
      [](auto& t, V v0, V v1) {
	t.verify_equal(simd::reduce(v0), T(0));
	t.verify_equal(simd::reduce(v1), T(V::size()));
      }
    };

    ADD_TEST(Product) {
      std::tuple {poisoned(0), poisoned(1)},
      [](auto& t, V v0, V v1) {
	t.verify_equal(simd::reduce(v0, mul), T(0));
	t.verify_equal(simd::reduce(v1, mul), T(1));
      }
    };

    ADD_TEST(UnknownSum) {
      std::tuple {poisoned(0), poisoned(1)},
      [](auto& t, V v0, V v1) {
	t.verify_equal(simd::reduce(v0, my_add), T(0));
	t.verify_equal(simd::reduce(v1, my_add), T(V::size()));
      }
    };

    ADD_TEST(And, std::is_integral_v<T>) {
      std::tuple {poisoned(0), poisoned(1)},
      [](auto& t, V v0, V v1) {
	t.verify_equal(simd::reduce(v0, bit_and), T(0));
	t.verify_equal(simd::reduce(v1, bit_and), T(1));
      }
    };

    ADD_TEST(Or, std::is_integral_v<T>) {
      std::tuple {poisoned(0), poisoned(1)},
      [](auto& t, V v0, V v1) {
	t.verify_equal(simd::reduce(v0, bit_or), T(0));
	t.verify_equal(simd::reduce(v1, bit_or), T(1));
      }
    };

    ADD_TEST(Xor, std::is_integral_v<T>) {
      std::tuple {poisoned(0), poisoned(1)},
      [](auto& t, V v0, V v1) {
	t.verify_equal(simd::reduce(v0, bit_xor), T(0));
	t.verify_equal(simd::reduce(v1, bit_xor), T(V::size() & 1));
      }
    };
  };

#include "create_tests.h"
