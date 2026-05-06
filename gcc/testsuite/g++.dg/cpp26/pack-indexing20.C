// PR c++/124198
// { dg-do compile { target c++17 } }
// { dg-options "-Wunused-but-set-variable" }

template <int ...N>
int
foo ()
{
  constexpr int a = 1;		// { dg-bogus "variable 'a' set but not used" }
  return N...[a];		// { dg-warning "pack indexing only available with" "" { target c++23_down } }
}

int a = foo <1, 2, 3> ();
