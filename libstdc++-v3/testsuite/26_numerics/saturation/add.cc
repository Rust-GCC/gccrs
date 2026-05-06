// { dg-do compile { target c++26 } }

// C++26 Saturation arithmetic [numerics.sat]

#include <numeric>
#include <limits>

template<typename T, typename U>
concept can_add_sat
  = requires(T t, U u) { { std::saturating_add(t, u) } -> std::same_as<T>; };

static_assert( can_add_sat<int, int> );
static_assert( not can_add_sat<int, short> );
static_assert( not can_add_sat<unsigned, int> );
static_assert( noexcept(std::saturating_add(0, 0)) );

using std::saturating_add;

// Signed type
static_assert(saturating_add(0, 0) == 0);
static_assert(saturating_add(1, 1) == 2);
static_assert(saturating_add(-1, -1) == -2);
static_assert(saturating_add(-1, 1) == 0);
constexpr auto max = std::numeric_limits<int>::max();
constexpr auto min = std::numeric_limits<int>::min();
static_assert(saturating_add(max, 1) == max);
static_assert(saturating_add(1, max) == max);
static_assert(saturating_add(max, max) == max);
static_assert(saturating_add(min, -1) == min);
static_assert(saturating_add(-1, min) == min);
static_assert(saturating_add(min, min) == min);
static_assert(saturating_add(max, min) == -1);
static_assert(saturating_add(min, max) == -1);

// Wider signed type than the args
static_assert(saturating_add<long long>(max, max) == (long long)max * 2);
static_assert(saturating_add<long long>(min, min) == (long long)min * 2);

// Signed type that undergoes integer promotion
constexpr auto shrt_max = std::numeric_limits<short>::max();
constexpr auto shrt_min = std::numeric_limits<short>::min();
static_assert(saturating_add<short>(0, 0) == 0);
static_assert(saturating_add<short>(1, 1) == 2);
static_assert(saturating_add<short>(shrt_max, shrt_max) == shrt_max);
static_assert(saturating_add<short>(shrt_max, 1) == shrt_max);
static_assert(saturating_add<short>(1, shrt_max) == shrt_max);
static_assert(saturating_add<short>(shrt_min, (short)-1) == shrt_min);
static_assert(saturating_add<short>((short)-1, shrt_min) == shrt_min);
static_assert(saturating_add<short>(shrt_min, (short)1) == -shrt_max);
static_assert(saturating_add<short>((short)1, shrt_min) == -shrt_max);

// Unsigned type
static_assert(saturating_add(0u, 0u) == 0u);
static_assert(saturating_add(1u, 1u) == 2u);
constexpr auto umax = std::numeric_limits<unsigned>::max();
static_assert(saturating_add(umax, 1u) == umax);
static_assert(saturating_add(1u, umax) == umax);
static_assert(saturating_add(umax, umax) == umax);
static_assert(saturating_add(0u, umax) == umax);
static_assert(saturating_add(umax, 0u) == umax);
static_assert(saturating_add(0u, 1u) == 1u);
static_assert(saturating_add(1u, 0u) == 1u);

// Wider unsigned type than the args
static_assert(saturating_add<unsigned long long>(umax, umax) == (long long)umax * 2);

// Unsigned type that undergoes integer promotion
constexpr auto ushrt_max = std::numeric_limits<unsigned short>::max();
static_assert(saturating_add<unsigned short>(0, 0) == 0);
static_assert(saturating_add<unsigned short>(1, 1) == 2);
static_assert(saturating_add<unsigned short>(ushrt_max, ushrt_max) == ushrt_max);
static_assert(saturating_add<unsigned short>(ushrt_max, 1) == ushrt_max);
static_assert(saturating_add<unsigned short>(1, ushrt_max) == ushrt_max);
