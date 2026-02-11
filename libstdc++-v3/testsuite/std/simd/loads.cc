// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }

#include "test_setup.h"
#include <numeric>

template <typename T, std::size_t N, std::size_t Alignment>
  class alignas(Alignment) aligned_array
    : public std::array<T, N>
  {};

template <typename V>
  struct Tests
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    static_assert(simd::alignment_v<V> <= 256);

    ADD_TEST(load_zeros) {
      std::tuple {aligned_array<T, V::size * 2, 256> {}, aligned_array<int, V::size * 2, 256> {}},
      [](auto& t, auto mem, auto ints) {
	t.verify_equal(simd::unchecked_load<V>(mem), V());
	t.verify_equal(simd::partial_load<V>(mem), V());

	t.verify_equal(simd::unchecked_load<V>(mem, simd::flag_aligned), V());
	t.verify_equal(simd::partial_load<V>(mem, simd::flag_aligned), V());

	t.verify_equal(simd::unchecked_load<V>(mem, simd::flag_overaligned<256>), V());
	t.verify_equal(simd::partial_load<V>(mem, simd::flag_overaligned<256>), V());

	t.verify_equal(simd::unchecked_load<V>(mem.begin() + 1, mem.end()), V());
	t.verify_equal(simd::partial_load<V>(mem.begin() + 1, mem.end()), V());
	t.verify_equal(simd::partial_load<V>(mem.begin() + 1, mem.begin() + 1), V());
	t.verify_equal(simd::partial_load<V>(mem.begin() + 1, mem.begin() + 2), V());

	t.verify_equal(simd::unchecked_load<V>(ints, simd::flag_convert), V());
	t.verify_equal(simd::partial_load<V>(ints, simd::flag_convert), V());

	t.verify_equal(simd::unchecked_load<V>(mem, M(true)), V());
	t.verify_equal(simd::unchecked_load<V>(mem, M(false)), V());
	t.verify_equal(simd::partial_load<V>(mem, M(true)), V());
	t.verify_equal(simd::partial_load<V>(mem, M(false)), V());
      }
    };

    static constexpr V ref = test_iota<V, 1, 0>;
    static constexpr V ref1 = V([](int i) { return i == 0 ? T(1): T(); });

    template <typename U>
    static constexpr auto
    make_iota_array()
    {
      aligned_array<U, V::size * 2, simd::alignment_v<V, U>> arr = {};
      U init = 0;
      for (auto& x : arr) x = (init += U(1));
      return arr;
    }

    ADD_TEST(load_iotas, requires {T() + T(1);}) {
      std::tuple {make_iota_array<T>(), make_iota_array<int>()},
      [](auto& t, auto mem, auto ints) {
	t.verify_equal(simd::unchecked_load<V>(mem), ref);
	t.verify_equal(simd::partial_load<V>(mem), ref);

	t.verify_equal(simd::unchecked_load<V>(mem.begin() + 1, mem.end()), ref + T(1));
	t.verify_equal(simd::partial_load<V>(mem.begin() + 1, mem.end()), ref + T(1));
	t.verify_equal(simd::partial_load<V>(mem.begin(), mem.begin() + 1), ref1);

	t.verify_equal(simd::unchecked_load<V>(mem, simd::flag_aligned), ref);
	t.verify_equal(simd::partial_load<V>(mem, simd::flag_aligned), ref);

	t.verify_equal(simd::unchecked_load<V>(ints, simd::flag_convert), ref);
	t.verify_equal(simd::partial_load<V>(ints, simd::flag_convert), ref);
	t.verify_equal(simd::partial_load<V>(
			 ints.begin(), ints.begin(), simd::flag_convert), V());
	t.verify_equal(simd::partial_load<V>(
			 ints.begin(), ints.begin() + 1, simd::flag_convert), ref1);

	t.verify_equal(simd::unchecked_load<V>(mem, M(true)), ref);
	t.verify_equal(simd::unchecked_load<V>(mem, M(false)), V());
	t.verify_equal(simd::partial_load<V>(mem, M(true)), ref);
	t.verify_equal(simd::partial_load<V>(mem, M(false)), V());
      }
    };

    static constexpr M alternating = M([](int i) { return 1 == (i & 1); });
    static constexpr V ref_k = select(alternating, ref, T());
    static constexpr V ref_2 = select(M([](int i) { return i < 2; }), ref, T());
    static constexpr V ref_k_2 = select(M([](int i) { return i < 2; }), ref_k, T());

    ADD_TEST(masked_loads) {
      std::tuple {make_iota_array<T>(), make_iota_array<int>(), alternating, M(true), M(false)},
      [](auto& t, auto mem, auto ints, M k, M tr, M fa) {
	t.verify_equal(simd::unchecked_load<V>(mem, tr), ref);
	t.verify_equal(simd::unchecked_load<V>(mem, fa), V());
	t.verify_equal(simd::unchecked_load<V>(mem, k), ref_k);

	t.verify_equal(simd::unchecked_load<V>(ints, tr, simd::flag_convert), ref);
	t.verify_equal(simd::unchecked_load<V>(ints, fa, simd::flag_convert), V());
	t.verify_equal(simd::unchecked_load<V>(ints, k, simd::flag_convert), ref_k);

	t.verify_equal(simd::partial_load<V>(mem, tr), ref);
	t.verify_equal(simd::partial_load<V>(mem, fa), V());
	t.verify_equal(simd::partial_load<V>(mem, k), ref_k);

	t.verify_equal(simd::partial_load<V>(mem.begin(), mem.begin() + 2, tr), ref_2);
	t.verify_equal(simd::partial_load<V>(mem.begin(), mem.begin() + 2, fa), V());
	t.verify_equal(simd::partial_load<V>(mem.begin(), mem.begin() + 2, k), ref_k_2);

	t.verify_equal(simd::partial_load<V>(ints.begin(), ints.begin() + 2, tr,
					     simd::flag_convert), ref_2);
	t.verify_equal(simd::partial_load<V>(ints.begin(), ints.begin() + 2, fa,
					     simd::flag_convert), V());
	t.verify_equal(simd::partial_load<V>(ints.begin(), ints.begin() + 2, k,
					     simd::flag_convert), ref_k_2);
      }
    };
  };

#include "create_tests.h"
