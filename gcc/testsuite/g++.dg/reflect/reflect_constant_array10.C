// LWG4483 - Multidimensional arrays are not supported by
// meta::reflect_constant_array and related functions.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <ranges>
#include <meta>

consteval auto
foo ()
{
  int a[3][3][3];
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 3; ++k)
	a[i][j][k] = i + 3 * j + 9 * k;
  std::span <int[3][3]> b = a;
  return std::meta::reflect_constant_array (b);
}

static_assert (foo () == foo ());
static_assert (type_of (foo ()) == ^^const int [3][3][3]);
constexpr auto &m = [: foo () :];
static_assert (foo () == std::meta::reflect_constant_array (m));
static_assert (foo () == std::meta::reflect_constant_array (std::span <const int[3][3]> (m)));
consteval {
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 3; ++k)
	if (m[i][j][k] != i + 3 * j + 9 * k)
	  throw 1;
}

consteval auto
bar ()
{
  int a[3][3][3];
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 3; ++k)
	a[i][j][k] = i + 3 * j + 9 * k;
  std::span <int[3][3]> b = a;
  return std::meta::reflect_constant_array (b | std::views::reverse);
}

static_assert (bar () == bar ());
static_assert (type_of (bar ()) == ^^const int [3][3][3]);
constexpr auto &n = [: bar () :];
static_assert (bar () == std::meta::reflect_constant_array (n));
static_assert (bar () == std::meta::reflect_constant_array (std::span <const int[3][3]> (n)));
consteval {
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 3; ++k)
	if (n[i][j][k] != (2 - i) + 3 * j + 9 * k)
	  throw 1;
}
