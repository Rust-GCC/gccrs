// PR c++/105667
// { dg-do compile { target c++20 } }

template <auto A = []<class B>(B){}>
struct C {
  using D = void;
};

template <class...>
using E = C<>::D;

using F = E<>;
