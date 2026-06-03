// PR c++/125490
// { dg-do compile { target c++20 } }

namespace N {
  struct A { };
}

constexpr bool operator==(N::A, auto) { return true; } // #1

template<class T>
concept C = T{} == T{};

constexpr bool operator==(N::A, N::A) { return false; } // #2

static_assert(C<N::A>); // must only consider #1
