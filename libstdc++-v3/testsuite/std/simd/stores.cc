// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }
// { dg-additional-options "-msse2" }

#include "test_setup.h"

template <typename V>
  struct Tests
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    static_assert(simd::alignment_v<V> <= 256);

    ADD_TEST(stores, requires {T() + T(1);}) {
      std::tuple {test_iota<V, 1, 0>, std::array<T, V::size * 2> {}, std::array<int, V::size * 2> {}},
      [](auto& t, const V v, const auto& mem_init, const auto& ints_init) {
	alignas(256) std::array<T, V::size * 2> mem = mem_init;
	alignas(256) std::array<int, V::size * 2> ints = ints_init;

	simd::unchecked_store(v, mem, simd::flag_aligned);
	simd::unchecked_store(v, mem.begin() + V::size(), mem.end());
	for (int i = 0; i < V::size; ++i)
	  {
	    t.verify_equal(mem[i], T(i + 1));
	    t.verify_equal(mem[V::size + i], T(i + 1));
	  }
#if VIR_NEXT_PATCH
	if constexpr (complex_like<T>)
	  {
	  }
	else
#endif
	  {
	    simd::unchecked_store(v, ints, simd::flag_convert);
	    simd::partial_store(v, ints.begin() + V::size() + 1, ints.end(),
			      simd::flag_convert | simd::flag_overaligned<alignof(int)>);
	    for (int i = 0; i < V::size; ++i)
	      {
		t.verify_equal(ints[i], int(T(i + 1)));
		t.verify_equal(ints[V::size + i], int(T(i)));
	      }

	    simd::unchecked_store(V(), ints.begin(), V::size(), simd::flag_convert);
	    simd::unchecked_store(V(), ints.begin() + V::size(), V::size(), simd::flag_convert);
	    for (int i = 0; i < 2 * V::size; ++i)
	      t.verify_equal(ints[i], 0)("i =", i);

	    if constexpr (V::size() > 1)
	      {
		simd::partial_store(v, ints.begin() + 1, V::size() - 2, simd::flag_convert);
		for (int i = 0; i < V::size - 2; ++i)
		  t.verify_equal(ints[i], int(T(i)));
		t.verify_equal(ints[V::size - 1], 0);
		t.verify_equal(ints[V::size], 0);
	      }
	    else
	      {
		simd::partial_store(v, ints.begin() + 1, 0, simd::flag_convert);
		t.verify_equal(ints[0], 0);
		t.verify_equal(ints[1], 0);
	      }
	  }
      }
    };
  };

#include "create_tests.h"
