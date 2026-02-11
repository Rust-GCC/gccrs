// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }

#include "test_setup.h"

template <typename V>
  struct Tests
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    ADD_TEST(VecCatChunk) {
      std::tuple{test_iota<V>, test_iota<V, 1>},
      [](auto& t, const V v0, const V v1) {
	auto c = cat(v0, v1);
	t.verify_equal(c.size(), V::size() * 2);
	for (int i = 0; i < V::size(); ++i)
	  t.verify_equal(c[i], v0[i])(i);
	for (int i = 0; i < V::size(); ++i)
	  t.verify_equal(c[i + V::size()], v1[i])(i);
	const auto [c0, c1] = simd::chunk<V>(c);
	t.verify_equal(c0, v0);
	t.verify_equal(c1, v1);
	if constexpr (V::size() <= 35)
	  {
	    auto d = cat(v1, c, v0);
	    for (int i = 0; i < V::size(); ++i)
	      {
		t.verify_equal(d[i], v1[i])(i);
		t.verify_equal(d[i + V::size()], v0[i])(i);
		t.verify_equal(d[i + 2 * V::size()], v1[i])(i);
		t.verify_equal(d[i + 3 * V::size()], v0[i])(i);
	      }
	    const auto [...chunked] = simd::chunk<3>(d);
	    t.verify_equal(cat(chunked...), d);
	  }
      }
    };

    ADD_TEST(MaskCatChunk) {
      std::tuple{M([](int i) { return 1 == (i & 1); }), M([](int i) { return 1 == (i % 3); })},
      [](auto& t, const M k0, const M k1) {
	auto c = cat(k0, k1);
	t.verify_equal(c.size(), V::size() * 2);
	for (int i = 0; i < V::size(); ++i)
	  t.verify_equal(c[i], k0[i])(i);
	for (int i = 0; i < V::size(); ++i)
	  t.verify_equal(c[i + V::size()], k1[i])(i);
	const auto [c0, c1] = simd::chunk<M>(c);
	t.verify_equal(c0, k0);
	t.verify_equal(c1, k1);
	if constexpr (V::size() <= 35)
	  {
	    auto d = cat(k1, c, k0);
	    for (int i = 0; i < V::size(); ++i)
	      {
		t.verify_equal(d[i], k1[i])(i);
		t.verify_equal(d[i + V::size()], k0[i])(i);
		t.verify_equal(d[i + 2 * V::size()], k1[i])(i);
		t.verify_equal(d[i + 3 * V::size()], k0[i])(i);
	      }
	    const auto [...chunked] = simd::chunk<3>(d);
	    t.verify_equal(cat(chunked...), d);
	  }
      }
    };
  };

#include "create_tests.h" // { dg-prune-output "Wpsabi" }
