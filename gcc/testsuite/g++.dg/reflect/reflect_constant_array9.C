// LWG4483 - Multidimensional arrays are not supported by
// meta::reflect_constant_array and related functions.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

consteval auto
foo ()
{
  int a[3][3][3];
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 3; ++k)
	a[i][j][k] = i + j + k;
  return std::meta::reflect_constant_array (a);
}

static_assert (foo () == foo ());
static_assert (type_of (foo ()) == ^^const int [3][3][3]);
constexpr auto &m = [: foo () :];
consteval {
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 3; ++k)
	if (m[i][j][k] != i + j + k)
	  throw 1;
}
