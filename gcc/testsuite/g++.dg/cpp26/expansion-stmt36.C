// CWG3140 - Allowing expansion over non-constant std::array
// { dg-do run { target c++17 } }
// { dg-options "" }

#include <array>

std::array <int, 5>
foo ()
{
  return { 1, 2, 3, 4, 5 };
}

int
main ()
{
  int r = 0;
  template for (auto n : foo ())	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    r += n;
  if (r != 15)
    __builtin_abort ();
}
