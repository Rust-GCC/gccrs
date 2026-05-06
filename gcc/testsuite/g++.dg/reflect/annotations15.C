// P3795R2 - Miscellaneous Reflection Cleanup
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

void foo ([[=1]] int x);
constexpr auto rf = parameters_of (^^foo)[0];
void foo ([[=2, =3]] int x);

void
foo ([[=4, =5]] int x)
{
  static_assert (annotations_of (^^x).size () == 2);
  static_assert ([: constant_of (annotations_of (^^x)[0]) :] == 4);
  static_assert ([: constant_of (annotations_of (^^x)[1]) :] == 5);
  static_assert (annotations_of (rf).size () == 5);
  static_assert ([: constant_of (annotations_of (rf)[0]) :] == 1);
  static_assert ([: constant_of (annotations_of (rf)[1]) :] == 2);
  static_assert ([: constant_of (annotations_of (rf)[2]) :] == 3);
  static_assert ([: constant_of (annotations_of (rf)[3]) :] == 4);
  static_assert ([: constant_of (annotations_of (rf)[4]) :] == 5);
  static_assert (annotations_of (variable_of (rf)).size () == 2);
  static_assert ([: constant_of (annotations_of (variable_of (rf))[0]) :] == 4);
  static_assert ([: constant_of (annotations_of (variable_of (rf))[1]) :] == 5);
  static_assert (annotations_of (parameters_of (^^foo)[0]).size () == 5);
  static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[0]) :] == 1);
  static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[1]) :] == 2);
  static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[2]) :] == 3);
  static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[3]) :] == 4);
  static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[4]) :] == 5);
  static_assert (annotations_of (^^x)[0] == annotations_of (rf)[3]);
  static_assert (annotations_of (^^x)[0] == annotations_of (variable_of (rf))[0]);
  static_assert (annotations_of (^^x)[0] == annotations_of (parameters_of (^^foo)[0])[3]);
}

void foo ([[=6]] int x);
static_assert (annotations_of (parameters_of (^^foo)[0]).size () == 6);
static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[0]) :] == 1);
static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[1]) :] == 2);
static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[2]) :] == 3);
static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[3]) :] == 4);
static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[4]) :] == 5);
static_assert ([: constant_of (annotations_of (parameters_of (^^foo)[0])[5]) :] == 6);
