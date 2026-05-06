// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }
// { dg-additional-options "-msse2" }

#include "test_setup.h"

template <typename V>
  requires (V::size() * sizeof(typename V::value_type) <= 70 * 4) // avoid exploding RAM usage
  struct Tests<V>
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    static constexpr int max = sizeof(T) == 8 ? 64 : 32;

    ADD_TEST_N(known_shift, 4, std::is_integral_v<T>) {
      std::tuple {test_iota<V>},
      []<int N>(auto& t, const V x) {
	constexpr int shift = max * (N + 1) / 4 - 1;
	constexpr T tmax = std::numeric_limits<T>::max();
	constexpr V vshift = T(shift);
	const V vshiftx = vshift ^ (x & std::cw<1>);
	t.verify(__is_const_known(vshift));

	V ref([&](int i) -> T { return x[i] >> shift; });
	V refx([&](int i) -> T { return x[i] >> (shift ^ (i & 1)); });
	t.verify_equal(x >> shift, ref)("{:d} >> {:d}", x, shift);
	t.verify_equal(x >> vshift, ref)("{:d} >> {:d}", x, vshift);
	t.verify_equal(x >> vshiftx, refx)("{:d} >> {:d}", x, vshiftx);

	const V y = ~x;
	ref = V([&](int i) -> T { return T(~x[i]) >> shift; });
	refx = V([&](int i) -> T { return T(~x[i]) >> (shift ^ (i & 1)); });
	t.verify_equal(y >> shift, ref)("{:d} >> {:d}", y, shift);
	t.verify_equal(y >> vshift, ref)("{:d} >> {:d}", y, vshift);
	t.verify_equal(y >> vshiftx, refx)("{:d} >> {:d}", y, vshiftx);

	const V z = tmax - x;
	ref = V([&](int i) -> T { return T(tmax - x[i]) >> shift; });
	refx = V([&](int i) -> T { return T(tmax - x[i]) >> (shift ^ (i & 1)); });
	t.verify_equal(z >> shift, ref)("{:d} >> {:d}", z, shift);
	t.verify_equal(z >> vshift, ref)("{:d} >> {:d}", z, vshift);
	t.verify_equal(z >> vshiftx, refx)("{:d} >> {:d}", z, vshiftx);
      }
    };

    ADD_TEST(unknown_shift, std::is_integral_v<T>) {
      std::tuple {test_iota<V>},
      [](auto& t, const V x) {
	for (int shift = 0; shift < max; ++shift)
	  {
	    constexpr T tmax = std::numeric_limits<T>::max();
	    const V vshift = T(shift);
	    const V vshiftx = vshift ^ (x & std::cw<1>);
	    t.verify(std::is_constant_evaluated()
		       || (!is_const_known(vshift) && !is_const_known(shift)));

	    V ref([&](int i) -> T { return x[i] >> shift; });
	    V refx([&](int i) -> T { return x[i] >> (shift ^ (i & 1)); });
	    t.verify_equal(x >> shift, ref)("{:d} >> {:d}", x, shift);
	    t.verify_equal(x >> vshift, ref)("{:d} >> {:d}", x, vshift);
	    t.verify_equal(x >> vshiftx, refx)("{:d} >> {:d}", x, vshiftx);

	    const V y = ~x;
	    ref = V([&](int i) -> T { return T(~x[i]) >> shift; });
	    refx = V([&](int i) -> T { return T(~x[i]) >> (shift ^ (i & 1)); });
	    t.verify_equal(y >> shift, ref)("{:d} >> {:d}", y, shift);
	    t.verify_equal(y >> vshift, ref)("{:d} >> {:d}", y, vshift);
	    t.verify_equal(y >> vshiftx, refx)("{:d} >> {:d}", y, vshiftx);

	    const V z = tmax - x;
	    ref = V([&](int i) -> T { return T(tmax - x[i]) >> shift; });
	    refx = V([&](int i) -> T { return T(tmax - x[i]) >> (shift ^ (i & 1)); });
	    t.verify_equal(z >> shift, ref)("{:d} >> {:d}", z, shift);
	    t.verify_equal(z >> vshift, ref)("{:d} >> {:d}", z, vshift);
	    t.verify_equal(z >> vshiftx, refx)("{:d} >> {:d}", z, vshiftx);
	  }
      }
    };
  };

template <typename V>
  struct Tests
  {};

void create_tests()
{
  template for (auto t : {char(), short(), unsigned(), 0l, 0ull})
    [[maybe_unused]] Tests<simd::vec<decltype(t)>> test;
  template for (constexpr int n : {1, 3, 17})
    [[maybe_unused]] Tests<simd::vec<int, n>> test;
}
