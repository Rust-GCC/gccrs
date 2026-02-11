// PR c++/123660
// PR c++/123661
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct s {
  int get_z(this s) { return 4; }
  static int get_y() { return 4; }
  int get_x() { return 3; }
  int xx;
  static int xxx;
};

int s::xxx = 5;

template<typename T, decltype(0uz) I>
constexpr auto test() {
  constexpr auto ctx = std::meta::access_context::current();
  return &[:members_of(^^T, ctx)[I]:];
}

static_assert(test<s, 0uz>() == &s::get_z);
static_assert(test<s, 1uz>() == &s::get_y);
static_assert(test<s, 2uz>() == &s::get_x);
static_assert(test<s, 3uz>() == &s::xx);
static_assert(test<s, 4uz>() == &s::xxx);
