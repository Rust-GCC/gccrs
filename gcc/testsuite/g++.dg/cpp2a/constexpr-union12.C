// PR c++/124910
// { dg-do compile { target c++20 } }

union U {
  int n;
  constexpr U() { }
};
constexpr U u; // { dg-bogus "" } U() is not a constant expression
