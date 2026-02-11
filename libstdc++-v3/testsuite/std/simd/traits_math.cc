// { dg-do compile { target c++26 } }
// { dg-require-effective-target x86 }

#include <simd>
#include <stdfloat>

namespace simd = std::simd;

// vec.math ///////////////////////////////////////

namespace math_tests
{
  using simd::__deduced_vec_t;
  using simd::__math_floating_point;
  using std::is_same_v;

  using vf2 = simd::vec<float, 2>;
  using vf4 = simd::vec<float, 4>;

  template <typename T0, typename T1>
    concept has_common_type = requires { typename std::common_type<T0, T1>::type; };

  template <typename T>
    concept has_deduced_vec = requires { typename simd::__deduced_vec_t<T>; };

  static_assert(!has_common_type<vf2, vf4>);
  static_assert( has_common_type<int, vf2>);

  template <typename T, bool Strict = false>
    struct holder
    {
      T value;

      constexpr
      operator const T&() const
      { return value; }

      template <typename U>
	requires (!std::same_as<T, U>) && Strict
	operator U() const = delete;
    };

  // The next always has a common_type because the UDT is convertible_to<float> and is not an
  // arithmetic type:
  static_assert( has_common_type<holder<int>, vf2>);

  // It's up to the UDT to constrain itself better:
  static_assert(!has_common_type<holder<int, true>, vf2>);

  // However, a strict UDT can still work
  static_assert( has_common_type<holder<float, true>, vf2>);

  // Except if it needs any kind of conversion, even if it's value-preserving. Again the semantics
  // are what the UDT defined.
  static_assert(!has_common_type<holder<short, true>, vf2>);

  static_assert(!has_deduced_vec<int>);
  static_assert(!__math_floating_point<int>);
  static_assert(!__math_floating_point<float>);
  static_assert(!__math_floating_point<simd::vec<int>>);
  static_assert( __math_floating_point<simd::vec<float>>);
}
