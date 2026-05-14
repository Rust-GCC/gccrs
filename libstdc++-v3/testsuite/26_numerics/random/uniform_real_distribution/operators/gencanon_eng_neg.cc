// { dg-do compile { target { c++11 } } }
// { dg-require-effective-target __float128 }
// { dg-require-effective-target base_quadfloat_support }
// { dg-add-options __float128 }

#include <random>
#include <cstdint>

template<std::uint64_t Max, typename Under = std::mt19937_64>
struct trimmed_engine
{
  using result_type = std::uint64_t;

  static constexpr
  result_type min()
  { return result_type(0); }

  static constexpr
  result_type max()
  { return result_type(Max); }

  trimmed_engine() : dist(min(), max())
  {}

  result_type operator()()
  { return dist(under); }

private:
  Under under;
  std::uniform_int_distribution<result_type> dist;
};

template<typename Real, size_t Bits>
void
test_non_pow2()
{
  trimmed_engine<(std::uint64_t(1) << Bits) - 2> non_pow2_engine;
  (void)std::generate_canonical<Real, -1u>(non_pow2_engine);
}

template<typename Real, size_t Bits>
void
test_pow2()
{
  trimmed_engine<(std::uint64_t(1) << Bits) - 1> pow2_engine;
  (void)std::generate_canonical<Real, -1u>(pow2_engine);
}

int main()
{
  // For 128bit floating points, generator emitting a range, which size is 
  // not power of two, but of width of B bits, such that for any N:
  // N * B < 113 (bits in ieee128)
  // (N+1) * B > 128
  // are not supported, as they would require integer with more than 127 bits.

  // N == 3: B in [43, 57)
  test_non_pow2<__float128, 42>(); // 3 calls
  test_non_pow2<__float128, 43>(); // { dg-error "from here" }
  test_non_pow2<__float128, 56>(); // { dg-error "from here" }
  test_non_pow2<__float128, 57>(); // 2 calls
  test_pow2<__float128, 43>();
  test_pow2<__float128, 56>();

  // N == 4: B in [33, 38)
  test_non_pow2<__float128, 32>(); // 4 calls
  test_non_pow2<__float128, 33>(); // { dg-error "from here" }
  test_non_pow2<__float128, 37>(); // { dg-error "from here" }
  test_non_pow2<__float128, 38>(); // 3 calls
  test_pow2<__float128, 33>();
  test_pow2<__float128, 37>();

  // N == 5: B in [26, 29)
  test_non_pow2<__float128, 25>(); // 5 calls
  test_non_pow2<__float128, 26>(); // { dg-error "from here" }
  test_non_pow2<__float128, 28>(); // { dg-error "from here" }
  test_non_pow2<__float128, 29>(); // 4 calls
  test_pow2<__float128, 26>();
  test_pow2<__float128, 28>();

  // N == 6: B == 22
  test_non_pow2<__float128, 21>(); // 6 calls
  test_non_pow2<__float128, 22>(); // { dg-error "from here" }
  test_non_pow2<__float128, 23>(); // 5 calls
  test_pow2<__float128, 22>();
}

// { dg-prune-output "no type named 'type' in 'struct std::__detail::_Select_uint_least_t" }
// { dg-prune-output "static assertion failed: sorry, would be too much trouble for a slow result" }
