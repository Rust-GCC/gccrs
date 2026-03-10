// PR c++/124399
// { dg-do compile { target c++26 } }
// { dg-options "-freflection" }

#include <meta>

template <class T>
  struct [[=42]] D { };

constexpr std::meta::info a1 = annotations_of (^^D <int>)[0];
constexpr std::meta::info a2 = annotations_of (^^D <char>)[0];
static_assert (a1 != a2);
static_assert (constant_of (a1) == constant_of (a2));
