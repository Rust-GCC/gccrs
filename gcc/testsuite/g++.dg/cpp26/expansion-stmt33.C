// PR c++/124488
// { dg-do compile { target c++11 } }
// { dg-options "" }

void
foo ()
{
  template for (constexpr int a : { 42 })	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    auto b = [] () { return a; };
}

void
bar (int x)
{
  template for (const int a : { 42, ++x, 5 })	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    const int b = a;
}
