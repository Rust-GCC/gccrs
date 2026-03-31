// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::data_member_spec.

#include <meta>

using namespace std::meta;

struct T { constexpr T () : t (42) {} constexpr int bar () const { return t; } protected: int t; };
constexpr T t;

constexpr auto a = data_member_spec (^^int, { .name = "a", .annotations = { ^^t } }); // { dg-error "'T' must be a cv-unqualified structural type that is not a reference type" }
