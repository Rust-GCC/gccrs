// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::define_aggregate.

#include <meta>

using namespace std::meta;

struct S { int a, b; };
struct T;

consteval
{
  constexpr int two = 2;
  [[=1]] int three = 3;
  constexpr int fourtytwo = 42;
  constexpr double fourtytwoandhalf = 42.5;
  data_member_options a = { .name = "_",
			    .annotations = { ^^two, reflect_constant (42),
					     annotations_of (^^three)[0],
					     reflect_constant (42.5),
					     reflect_constant (S { 1, 2 }) } };
  auto dmsa = data_member_spec (^^int, a);
  define_aggregate (^^T, { dmsa });
}
consteval
{
  static_assert (annotations_of (^^T::_).size () == 5);
  static_assert ([: constant_of (annotations_of (^^T::_)[0]) :] == 2);
  static_assert ([: constant_of (annotations_of (^^T::_)[1]) :] == 42);
  static_assert ([: constant_of (annotations_of (^^T::_)[2]) :] == 1);
  static_assert ([: constant_of (annotations_of (^^T::_)[3]) :] == 42.5);
  static_assert ([: constant_of (annotations_of (^^T::_)[4]) :].a == 1);
  static_assert ([: constant_of (annotations_of (^^T::_)[4]) :].b == 2);
}
