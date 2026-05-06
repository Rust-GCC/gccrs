// PR c++/123660
// PR c++/123661
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

void f() {}
struct s {
  int get_z(this s) { return 4; }
  static int get_y() { return 4; }
  int get_x() { return 3; }
  int xx;
  static int xxx;
};

int s::xxx = 5;

template<std::meta::info refl_expr>
constexpr auto test() {
  return &[:refl_expr:];
}

static_assert(test<^^s::get_z>() == &s::get_z);
static_assert(test<^^s::get_y>() == &s::get_y);
static_assert(test<^^s::get_x>() == &s::get_x);
static_assert(test<^^s::xx>() == &s::xx);
static_assert(test<^^s::xxx>() == &s::xxx);
static_assert(test<^^f>() == &f);
