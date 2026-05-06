// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct A {};
struct B {};
struct C : A, B {};
struct D : A, B {};
constexpr auto ctx = std::meta::access_context::unchecked ();
static_assert (bases_of (^^C, ctx)[0] != bases_of (^^C, ctx)[1]);
static_assert (bases_of (^^C, ctx)[1] != bases_of (^^D, ctx)[1]);
