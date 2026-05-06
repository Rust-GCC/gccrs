// PR c++/124927
// { dg-do run { target c++23 } }
// { dg-options "" }

#include <array>
#include <ranges>

constexpr std::array a { 1, 2, 3, 4, 5 };
constexpr std::array b { 5, 4, 3, 2, 1 };

int
foo (int i)
{
  template for (constexpr auto z : std::views::zip (a, b)) // { dg-warning "'template for' only available with" "" { target c++23_down } }
    if (i == get <0> (z))
      return get <1> (z);
  return 0;
}

int
main ()
{
  for (int i = 1; i <= 5; ++i)
    if (foo (i) != 6 - i)
      __builtin_abort ();
  if (foo (0) != 0 || foo (6) != 0)
    __builtin_abort ();
}
