// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }
// { dg-additional-options "-msse2" }

#include "test_setup.h"
#include <utility>

template <typename V>
  struct Tests
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    static constexpr M alternating = M([](int i) { return 1 == (i & 1); });
    static constexpr M k010 = M([](int i) { return 1 == (i % 3); });
    static constexpr M k00111 = M([](int i) { return 2 < (i % 5); });

    ADD_TEST(mask_conversion) {
      std::array {alternating, k010, k00111},
      [](auto& t, M k) {
	template for (auto tmp : {char(), short(), int(), double()})
	  {
	    using U = decltype(tmp);
	    using M2 = simd::rebind_t<U, M>;
	    using M3 = simd::mask<U, V::size()>;
	    const M2 ref2 = M2([&](int i) { return k[i]; });
	    t.verify_equal(M2(k), ref2);
	    t.verify_equal(M(M2(k)), k);
	    if constexpr (!std::is_same_v<M2, M3>)
	      {
		const M3 ref3 = M3([&](int i) { return k[i]; });
		t.verify_equal(M3(k), ref3);
		t.verify_equal(M(M3(k)), k);
		t.verify_equal(M2(M3(k)), ref2);
		t.verify_equal(M3(M2(k)), ref3);
	      }
	  }
      }
    };

    ADD_TEST(mask_reductions_sanity) {
      std::tuple {M(true)},
      [](auto& t, M x) {
	t.verify_equal(std::simd::reduce_min_index(x), 0);
	t.verify_equal(std::simd::reduce_max_index(x), V::size - 1);
	t.verify_precondition_failure("An empty mask does not have a min_index.", [&] {
	  std::simd::reduce_min_index(!x);
	});
	t.verify_precondition_failure("An empty mask does not have a max_index.", [&] {
	  std::simd::reduce_max_index(!x);
	});
      }
    };

    ADD_TEST(mask_reductions) {
      std::tuple{test_iota<V>, test_iota<V> == T(0)},
      [](auto& t, V v, M k0) {
	// Caveat:
	// k0[n0 * (test_iota_max<V> + 1)] is true if it exists
	// k[n * (test_iota_max<V> + 1) + i] is true if it exists
	// none_of(k) is true if i > test_iota_max<V>
	// by construction of test_iota_max:
	static_assert(test_iota_max<V> < V::size());
	for (int i = 0; i < int(test_iota_max<V>) + 1; ++i)
	  {
	    M k = v == T(i);

	    const int nk = 1 + (V::size() - i - 1) / (test_iota_max<V> + 1);
	    const int maxk = (nk - 1) * (test_iota_max<V> + 1) + i;
	    t.verify(maxk < V::size());

	    const int nk0 = 1 + (V::size() - 1) / (test_iota_max<V> + 1);
	    const int maxk0 = (nk0 - 1) * (test_iota_max<V> + 1);
	    t.verify(maxk0 < V::size());

	    const int maxkork0 = std::max(maxk, maxk0);

	    t.verify_equal(k[i], true);
	    t.verify_equal(std::as_const(k)[i], true);
	    t.verify_equal(std::simd::reduce_min_index(k), i)(k);
	    t.verify_equal(std::simd::reduce_max_index(k), maxk)(k);
	    t.verify_equal(std::simd::reduce_min_index(k || k0), 0);
	    t.verify_equal(std::simd::reduce_max_index(k || k0), maxkork0);
	    t.verify_equal(k, k);
	    t.verify_not_equal(!k, k);
	    t.verify_equal(k | k, k);
	    t.verify_equal(k & k, k);
	    t.verify(none_of(k ^ k));
	    t.verify_equal(std::simd::reduce_count(k), nk);
	    if constexpr (sizeof(T) <= sizeof(0ULL))
	      t.verify_equal(-std::simd::reduce(-k), nk)(k)(-k);
	    t.verify_equal(std::simd::reduce_count(!k), V::size - nk)(!k);
	    if constexpr (V::size <= 128 && sizeof(T) <= sizeof(0ULL))
	      t.verify_equal(-std::simd::reduce(-!k), V::size - nk)(-!k);
	    t.verify(any_of(k));
	    t.verify(bool(any_of(k & k0) ^ (i != 0)));
	    k = M([&](int j) { return j == 0 ? true : k[j]; });
	    t.verify_equal(k[i], true);
	    t.verify_equal(std::as_const(k)[i], true);
	    t.verify_equal(k[0], true);
	    t.verify_equal(std::as_const(k)[0], true);
	    t.verify_equal(std::simd::reduce_min_index(k), 0)(k);
	    t.verify_equal(std::simd::reduce_max_index(k), maxk)(k);
	  }
      }
    };
  };

#include "create_tests.h" // { dg-prune-output "Wpsabi" }
