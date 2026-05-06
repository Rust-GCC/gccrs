// CWG3140 - Allowing expansion over non-constant std::array
// { dg-do run { target c++17 } }
// { dg-options "" }
// { dg-additional-options "-frange-for-ext-temps" { target c++20_down } }

#include <array>

struct A {
  A () { a++; }
  A (const A &) { a++; }
  ~A () { a--; }
  static int a;
};
int A::a = 0;

std::array <int, 5>
foo (A, A, A)
{
  return { 1, 2, 3, 4, 5 };
}

int
main ()
{
  int r = 0;
  if (A::a != 0)
    __builtin_abort ();
  template for (auto n : foo (A (), A (), A ()))	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    {
      if (A::a != 3)
	__builtin_abort ();
      r += n;
    }
  if (r != 15 || A::a != 0)
    __builtin_abort ();
}
