// CWG3140 - Allowing expansion over non-constant std::array
// { dg-do compile { target c++17 } }
// { dg-options "" }

#include <array>

std::array <int, 5>
foo ()
{
  return { 1, 2, 3, 4, 5 };
}

void
bar ()
{
  int r = 0;
  template for (constexpr auto n : foo ())	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    r += n;					// { dg-error " call to non-'constexpr' function" "" { target *-*-* } .-1 }
}
