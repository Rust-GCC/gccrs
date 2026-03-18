// PR c++/124177
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

constexpr int f() { return 42; }
[[=f]] void g() {}

static_assert (annotations_of (^^g).size () == 1);
static_assert (extract<int(*)()> (annotations_of (^^g)[0])() == 42);

constexpr int x[5]{};
[[=x]] void h() {}

static_assert (annotations_of (^^h).size () == 1);
static_assert (type_of (annotations_of (^^h)[0]) == ^^const int*);
