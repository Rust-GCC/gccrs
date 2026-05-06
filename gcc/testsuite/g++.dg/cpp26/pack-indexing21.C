// PR c++/124198
// { dg-do compile { target c++17 } }
// { dg-options "" }

struct A {
  constexpr A () : a (1) {}
  int a;
  constexpr operator int () const { return a; }
};

template <int ...N>
int
foo ()
{
  constexpr A a;
  return N...[a];		// { dg-warning "pack indexing only available with" "" { target c++23_down } }
}

int a = foo <1, 2, 3> ();
