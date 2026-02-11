// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }

#include "test_setup.h"
#include <utility>

namespace simd = std::simd;

template <std::size_t B, typename A>
  consteval std::size_t
  element_size(const simd::basic_mask<B, A>&)
  { return B; }

template <typename V>
  struct Tests
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    ADD_TEST(Sanity) {
      std::tuple{M([](int i) { return 1 == (i & 1); })},
      [](auto& t, const M k) {
	t.verify_equal(element_size(k), sizeof(T));
	for (int i = 0; i < k.size(); i += 2)
	  t.verify_equal(k[i], false)(k);
	for (int i = 1; i < k.size(); i += 2)
	  t.verify_equal(k[i], true)(k);
      }
    };

    ADD_TEST(Reductions) {
      std::tuple{M([](int i) { return 1 == (i & 1); }), M(true), M(false)},
      [](auto& t, const M k, const M tr, const M fa) {
	t.verify(!all_of(k))(k);
	if constexpr (V::size() > 1)
	  {
	    t.verify(any_of(k))(k);
	    t.verify(!none_of(k))(k);
	  }

	t.verify(all_of(tr));
	t.verify(any_of(tr));
	t.verify(!none_of(tr));

	t.verify(!all_of(fa));
	t.verify(!any_of(fa));
	t.verify(none_of(fa));
      }
    };

    ADD_TEST(CvtToInt, (sizeof(T) <= sizeof(0ull))) {
      std::tuple{M([](int i) { return 1 == (i & 1); }), M(true), M(false), M([](int i) {
		   return i % 13 == 0 || i % 7 == 0;
      })},
      [](auto& t, const M k, const M tr, const M fa, const M k2) {
	t.verify_equal(V(+tr), V(1));
	t.verify_equal(V(+fa), V());
	t.verify_equal(V(+k), init_vec<V, 0, 1>);

	if constexpr (std::is_integral_v<T>)
	  {
	    t.verify_equal(V(~tr), ~V(1));
	    t.verify_equal(V(~fa), ~V(0));
	    t.verify_equal(V(~k), ~init_vec<V, 0, 1>);
	  }

	t.verify(all_of(simd::rebind_t<char, M>(tr)));
	t.verify(!all_of(simd::rebind_t<char, M>(fa)));
	t.verify(!all_of(simd::rebind_t<char, M>(k)));

	t.verify_equal(fa.to_ullong(), 0ull);
	t.verify_equal(fa.to_bitset(), std::bitset<V::size()>());

	// test whether 'M -> bitset -> M' is an identity transformation
	t.verify_equal(M(fa.to_bitset()), fa)(fa.to_bitset());
	t.verify_equal(M(tr.to_bitset()), tr)(tr.to_bitset());
	t.verify_equal(M(k.to_bitset()), k)(k.to_bitset());
	t.verify_equal(M(k2.to_bitset()), k2)(k2.to_bitset());

	static_assert(sizeof(0ull) * CHAR_BIT == 64);
	if constexpr (V::size() <= 64)
	  {
	    constexpr unsigned long long full = -1ull >> (64 - V::size());
	    t.verify_equal(tr.to_ullong(), full)(std::hex, tr.to_ullong(), '^', full, "->",
						 tr.to_ullong() ^ full);
	    t.verify_equal(tr.to_bitset(), full);

	    constexpr unsigned long long alternating = 0xaaaa'aaaa'aaaa'aaaaULL & full;
	    t.verify_equal(k.to_ullong(), alternating)(std::hex, k.to_ullong(), '^', alternating,
						       "->", k.to_ullong() ^ alternating);
	    t.verify_equal(k.to_bitset(), alternating);

	    // 0, 7, 13, 14, 21, 26, 28, 35, 39, 42, 49, 52, 56, 63, 65, ...
	    constexpr unsigned long long bits7_13 = 0x8112'0488'1420'6081ULL & full;
	    t.verify_equal(k2.to_ullong(), bits7_13)(std::hex, k2.to_ullong());
	  }
	else
	  {
	    constexpr unsigned long long full = -1ull;
	    constexpr unsigned long long alternating = 0xaaaa'aaaa'aaaa'aaaaULL;
	    int shift = M::size() - 64;
	    t.verify_equal((tr.to_bitset() >> shift).to_ullong(), full);
	    t.verify_equal((k.to_bitset() >> shift).to_ullong(), alternating);
	  }

	t.verify_equal(+tr, -(-tr));
	t.verify_equal(-+tr, -tr);
      }
    };
  };

#include "create_tests.h" // { dg-prune-output "Wpsabi" }
