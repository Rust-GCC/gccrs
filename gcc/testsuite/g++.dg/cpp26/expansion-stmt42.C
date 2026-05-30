// PR c++/125123
// { dg-do compile { target c++20 } }
// { dg-options "-g" }

struct A
{
  int a[2];
  constexpr int const *begin () const { return a; }
  constexpr int const *end () const { return a + 2; }
};

template <typename T>
requires true
void
foo ()
{
  template for (constexpr int i : A { 0, 1 })	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    ;
  template for (constexpr int i : A { 0, 1 })	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    ;
}
template void foo <void> ();
