// LWG4483 - Multidimensional arrays are not supported by
// meta::reflect_constant_array and related functions.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::define_static_object.

#include <meta>

constexpr int arr[]{1, 2, 3};
constexpr const int (*ptr)[3] = std::define_static_object (arr);
static_assert (*ptr == std::define_static_array (arr).data ());
static_assert (ptr == &[: std::meta::constant_of (^^arr) :]);

constexpr int marr[3][3]{1, 2, 3};
constexpr const int (*mptr)[3][3] = std::define_static_object (marr);
static_assert (*mptr == std::define_static_array (marr).data ());
static_assert (mptr == &[: std::meta::constant_of (^^marr) :]);

union U { int a; long long b; };
constexpr U u = { .a = 42 };
constexpr const U *up = std::define_static_object (u);
static_assert (up->a == 42);
constexpr U v = { .b = 43LL };
constexpr const U *vp = std::define_static_object (v);
static_assert (vp->b == 43LL);
