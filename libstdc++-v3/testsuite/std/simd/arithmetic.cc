// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }
// { dg-additional-options "-msse2" }

#include "test_setup.h"

static constexpr bool is_iec559 =
#ifdef __GCC_IEC_559
      __GCC_IEC_559 >= 2;
#elif defined __STDC_IEC_559__
      __STDC_IEC_559__ == 1;
#else
      false;
#endif

#if VIR_NEXT_PATCH
template <typename V>
  requires complex_like<typename V::value_type>
  struct Tests<V>
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;
    using Real = typename T::value_type;
    using RealV = simd::rebind_t<Real, V>;

    static_assert(std::is_floating_point_v<Real>);

    static constexpr T min = std::numeric_limits<Real>::lowest();
    static constexpr T norm_min = std::numeric_limits<Real>::min();
    static constexpr T denorm_min = std::numeric_limits<Real>::denorm_min();
    static constexpr T max = std::numeric_limits<Real>::max();
    static constexpr T inf = std::numeric_limits<Real>::infinity();

    ADD_TEST(plus_minus) {
      std::tuple {V(), init_vec<V, C(1, 1), C(2, 2), C(3, 3)>},
      [](auto& t, V x, V y) {
	t.verify_equal(x + x, x);
	t.verify_equal(x - x, x);
	t.verify_equal(x + y, y);
	t.verify_equal(y + x, y);
	t.verify_equal(x - y, -y);
	t.verify_equal(y - x, y);
	t.verify_equal(x += T(1, -2), T(1, -2));
	t.verify_equal(x = x + x, T(2, -4));
	t.verify_equal(x = x - y, init_vec<V, C(1, -5), C(0, -6), C(-1, -7)>);
	t.verify_equal(x, init_vec<V, C(1, -5), C(0, -6), C(-1, -7)>);
      }
    };

    // complex multiplication & division has an edge case which is due to '-0. - -0.'. If we
    // interpret negative zero to represent a value between denorm_min and 0 (exclusive) then we
    // cannot know whether the resulting zero is negative or positive. ISO 60559 simply defines the
    // result to be positive zero, but that's throwing away half of the truth.
    //
    // Consider (https://compiler-explorer.com/z/61cYhrE48):
    // sqrt(x * complex{1.}) -> {0, +/-1}.
    // The sign of the imaginary part depends on whether x is double{-1} or complex{-1.}. This is
    // due to the type of the operand influencing the formula used for multiplication:
    //
    // 1. 'x * (u+iv)' is implemented as 'xu + i(xv)'
    //
    // 2. '(x+iy) * (u+iv)' is implemented as '(xu-yv) + i(xv+yu)'
    //
    // 'xv' is equal to -0 and 'yu' is equal to +0. Consequently the imaginary part in (1.) is -0
    // and in (2.) it is (-0 + 0) which is +0. The example above then uses that difference to hit
    // the branch cut on sqrt.

    // (x+iy)(u+iv) = (xu-yv)+i(xv+yu)
    // depending on FMA contraction or FLT_EVAL_METHOD 'inf - inf' can be 0, inf, -inf, or NaN (no
    // contraction).
    //
    // Because of all these issues, verify_equal is implemented to interpret "an infinity" as equal
    // to another infinity according to the interpretation of C23 Annex G.3.

    ADD_TEST(multiplication_corner_cases) {
      std::array {min, norm_min, denorm_min, max, inf},
      [](auto& t, V x) {
	t.verify_equal(x * x, x[0] * x[0]);
	const V y = x * T(1, 1);
	t.verify_equal(y * y, y[0] * y[0])(y);
	x *= T(0, 1);
	t.verify_equal(x * x, x[0] * x[0]);
	x *= T(1, 1);
	t.verify_equal(x * x, x[0] * x[0])(x);
	x *= T(1, Real(.5));
	t.verify_equal(x * x, x[0] * x[0])(x);
      }
    };

    ADD_TEST(multiplication) {
      std::tuple {V(), V(RealV(Real(1)), RealV()), V(RealV(), RealV(Real(1))),
		  init_vec<V, C(0, 2), C(2, 0), C(-1, 2)>},
      [](auto& t, V x, V one, V I, V z) {
	t.verify_equal(x * x, x);
	t.verify_equal(x * z, x);
	t.verify_equal(z * x, x);
	t.verify_equal(one * one, one);
	t.verify_equal(one * z, z);
	t.verify_equal(z * one, z);

	// Notes:
	// inf + -inf -> NaN
	//  0. + -0.  -> 0. (this is arbitrary, why not NaN: indeterminable sign?)
	// complex(0.) * -complex(2., 2.) -> (0, -0)
	//         0.  * -complex(2., 2.) -> (-0, -0)
	//  => the *type* of the operand determines the sign of the zero, which is *impossible*
	//     to implement with vec<complex>!
	// complex(DBL_MAX, DBL_MAX) * complex(2., 2.) -> (-nan, inf) => θ got lost
	// complex(1.) / complex(0., 0.) -> (inf, -nan) => θ got lost
	// complex(1.) / complex(-0., 0.) -> (inf, -nan) => θ got lost
	// complex(1.) / complex(0., -0.) -> (inf, -nan) => θ got lost
	// complex(1.) / complex(-DBL_INF, 0.) -> (-0, -0) => θ is wrong

	t.verify_bit_equal(one * I, I);

	// (0+i0) * (-0-i0) -> (-0 + 0) + i(-0 + -0) -> 0-i0
	t.verify_bit_equal(x * -x, T() * -T());
	t.verify_bit_equal(-x * x, -T() * T());

	t.verify_bit_equal(x * conj(x), T() * conj(T()));
	t.verify_bit_equal(x * -conj(x), T() * -conj(T()));

	// real * complex has extra overloads on complex but not on vec<complex>
	// for vec<complex> the result therefore needs to be "bit equal" only to
	// complex * complex
	t.verify_equal(x.real() * -x, T().real() * -T());
	t.verify_bit_equal(x.real() * -x, T() * -T());

	t.verify_bit_equal(I * one, I);
	t.verify_bit_equal(I * I, T(-1, 0));
	t.verify_bit_equal(z * I, init_vec<V, C(-2, 0), C(0., 2.), C(-2, -1)>);
	t.verify_bit_equal(std::complex{-0., 0.} * std::complex{0., 1.}, std::complex{-0., 0.});
	t.verify_bit_equal(std::complex{-0., -1.} * std::complex{0., 0.}, std::complex{0., -0.});
	t.verify_bit_equal(0. + -0., 0.);
      }
    };
  };
#endif

template <typename V>
  struct Tests
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    static constexpr T min = std::numeric_limits<T>::lowest();
    static constexpr T norm_min = std::numeric_limits<T>::min();
    static constexpr T max = std::numeric_limits<T>::max();

    ADD_TEST(plus0, requires(T x) { x + x; }) {
      std::tuple{V(), init_vec<V, 1, 2, 3, 4, 5, 6, 7>},
      [](auto& t, V x, V y) {
	t.verify_equal(x + x, x);
	t.verify_equal(x = x + T(1), T(1));
	t.verify_equal(x + x, T(2));
	t.verify_equal(x = x + y, init_vec<V, 2, 3, 4, 5, 6, 7, 8>);
	t.verify_equal(x = x + -y, T(1));
	t.verify_equal(x += y, init_vec<V, 2, 3, 4, 5, 6, 7, 8>);
	t.verify_equal(x, init_vec<V, 2, 3, 4, 5, 6, 7, 8>);
	t.verify_equal(x += -y, T(1));
	t.verify_equal(x, T(1));
      }
    };

    ADD_TEST(plus1, requires(T x) { x + x; }) {
      std::tuple{test_iota<V>},
      [](auto& t, V x) {
	t.verify_equal(x + std::cw<0>, x);
	t.verify_equal(std::cw<0> + x, x);
	t.verify_equal(x + T(), x);
	t.verify_equal(T() + x, x);
	t.verify_equal(x + -x, V());
	t.verify_equal(-x + x, V());
      }
    };

    ADD_TEST(minus0, requires(T x) { x - x; }) {
      std::tuple{T(1), T(0), init_vec<V, 1, 2, 3, 4, 5, 6, 7>},
      [](auto& t, V x, V y, V z) {
	t.verify_equal(x - y, x);
	t.verify_equal(x - T(1), y);
	t.verify_equal(y, x - T(1));
	t.verify_equal(x - x, y);
	t.verify_equal(x = z - x, init_vec<V, 0, 1, 2, 3, 4, 5, 6>);
	t.verify_equal(x = z - x, T(1));
	t.verify_equal(z -= x, init_vec<V, 0, 1, 2, 3, 4, 5, 6>);
	t.verify_equal(z, init_vec<V, 0, 1, 2, 3, 4, 5, 6>);
	t.verify_equal(z -= z, V());
	t.verify_equal(z, V());
      }
    };

    ADD_TEST(minus1, requires(T x) { x - x; }) {
      std::tuple{test_iota<V>},
      [](auto& t, V x) {
	t.verify_equal(x - x, V());
	t.verify_equal(x - std::cw<0>, x);
	t.verify_equal(std::cw<0> - x, -x);
	t.verify_equal(x - T(), x);
	t.verify_equal(T() - x, -x);
      }
    };

    ADD_TEST(times0, requires(T x) { x * x; }) {
      std::tuple{T(0), T(1), T(2)},
      [](auto& t, T v0, T v1, T v2) {
	V x = v1;
	V y = v0;
	t.verify_equal(x * y, y);
	t.verify_equal(x = x * T(2), T(2));
	t.verify_equal(x * x, T(4));
	y = init_vec<V, 1, 2, 3, 4, 5, 6, 7>;
	t.verify_equal(x = x * y, init_vec<V, 2, 4, 6, 8, 10, 12, 14>);
	y = v2;
	// don't test norm_min/2*2 in the following. There's no guarantee, in
	// general, that the result isn't flushed to zero (e.g. NEON without
	// subnormals)
	for (T n : {T(max - T(1)), std::is_floating_point_v<T> ? T(norm_min * T(3)) : min})
	  {
	    x = T(n / 2);
	    t.verify_equal(x * y, V(n));
	  }
	if (std::is_integral<T>::value && std::is_unsigned<T>::value)
	  {
	    // test modulo arithmetics
	    T n = max;
	    x = n;
	    for (T m : {T(2), T(7), T(max / 127), max})
	      {
		y = m;
		// if T is of lower rank than int, `n * m` will promote to int
		// before executing the multiplication. In this case an overflow
		// will be UB (and ubsan will warn about it). The solution is to
		// cast to uint in that case.
		using U
		  = std::conditional_t<(sizeof(T) < sizeof(int)), unsigned, T>;
		t.verify_equal(x * y, V(T(U(n) * U(m))));
	      }
	  }
	x = v2;
	t.verify_equal(x *= init_vec<V, 1, 2, 3>, init_vec<V, 2, 4, 6>);
	t.verify_equal(x, init_vec<V, 2, 4, 6>);
      }
    };

    ADD_TEST(times1, requires(T x) { x * x; }) {
      std::tuple{test_iota<V, 0, 11>},
      [](auto& t, V x) {
	t.verify_equal(x * x, V([](int i) { return T(T(i % 12) * T(i % 12)); }));
	t.verify_equal(x * std::cw<1>, x);
	t.verify_equal(std::cw<1> * x, x);
	t.verify_equal(x * T(1), x);
	t.verify_equal(T(1) * x, x);
	t.verify_equal(x * T(-1), -x);
	t.verify_equal(T(-1) * x, -x);
      }
    };

    // avoid testing subnormals and expect minor deltas for non-IEC559 float
    ADD_TEST(divide0, std::is_floating_point_v<T> && !is_iec559) {
      std::tuple{T(2), init_vec<V, 1, 2, 3, 4, 5, 6, 7>},
      [](auto& t, V x, V y) {
	t.verify_equal_to_ulp(x / x, V(T(1)), std::cw<1>);
	t.verify_equal_to_ulp(T(3) / x, V(T(3) / T(2)), std::cw<1>);
	t.verify_equal_to_ulp(x / T(3), V(T(2) / T(3)), std::cw<1>);
	t.verify_equal_to_ulp(y / x, init_vec<V, .5, 1, 1.5, 2, 2.5, 3, 3.5>, std::cw<1>);
      }
    };

    // avoid testing subnormals and expect minor deltas for non-IEC559 float
    ADD_TEST(divide1, std::is_floating_point_v<T> && !is_iec559) {
      std::array{T{norm_min * 1024}, T{1}, T{}, T{-1}, T{max / 1024}, T{max / T(4.1)}, max, min},
      [](auto& t, V a) {
	V b = std::cw<2>;
	V ref([&](int i) { return a[i] / 2; });
	t.verify_equal_to_ulp(a / b, ref, std::cw<1>);
	a = select(a == std::cw<0>, T(1), a);
	// -freciprocal-math together with flush-to-zero makes
	// the following range restriction necessary (i.e.
	// 1/|a| must be >= min). Intel vrcpps and vrcp14ps
	// need some extra slack (use 1.1 instead of 1).
	a = select(fabs(a) >= T(1.1) / norm_min, T(1), a);
	t.verify_equal_to_ulp(a / a, V(std::cw<1>), std::cw<1>)("\na = ", a);
	ref = V([&](int i) { return 2 / a[i]; });
	t.verify_equal_to_ulp(b / a, ref, std::cw<1>)("\na = ", a);
	t.verify_equal_to_ulp(b /= a, ref, std::cw<1>);
	t.verify_equal_to_ulp(b, ref, std::cw<1>);
      }
    };

    ADD_TEST(divide2, (is_iec559 || !std::is_floating_point_v<T>) && requires(T x) { x / x; }) {
      std::tuple{T(2), init_vec<V, 1, 2, 3, 4, 5, 6, 7>, init_vec<V, T(max), T(norm_min)>,
		 init_vec<V, T(norm_min), T(max)>, init_vec<V, T(max), T(norm_min) + 1>},
      [](auto& t, V x, V y, V z, V a, V b) {
	t.verify_equal(x / x, T(1));
	t.verify_equal(T(3) / x, T(T(3) / T(2)));
	t.verify_equal(x / T(3), T(T(2) / T(3)));
	t.verify_equal(y / x, init_vec<V, .5, 1, 1.5, 2, 2.5, 3, 3.5>);
	V ref = init_vec<V, T(max / 2), T(norm_min / 2)>;
	t.verify_equal(z / x, ref);
	ref = init_vec<V, T(norm_min / 2), T(max / 2)>;
	t.verify_equal(a / x, ref);
	t.verify_equal(b / b, T(1));
	ref = init_vec<V, T(2 / max), T(2 / (norm_min + 1))>;
	t.verify_equal(x / b, ref);
	t.verify_equal(x /= b, ref);
	t.verify_equal(x, ref);
      }
    };

    static constexpr V from0 = test_iota<V, 0, 63>;
    static constexpr V from1 = test_iota<V, 1, 64>;
    static constexpr V from2 = test_iota<V, 2, 65>;

    ADD_TEST(incdec, requires(T x) { ++x; x++; --x; x--; }) {
      std::tuple{from0},
      [](auto& t, V x) {
	t.verify_equal(x++, from0);
	t.verify_equal(x, from1);
	t.verify_equal(++x, from2);
	t.verify_equal(x, from2);

	t.verify_equal(x--, from2);
	t.verify_equal(x, from1);
	t.verify_equal(--x, from0);
	t.verify_equal(x, from0);
      }
    };
  };

#include "create_tests.h"
