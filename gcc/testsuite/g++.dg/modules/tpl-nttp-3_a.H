// PR c++/124953
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fmodule-header" }

template <class T> struct wrap { using type = T; };
template <auto V> using unwrap = decltype(V)::type;

struct Foo {
  struct Storage {};
  unwrap<wrap<Storage>{}> data_{};
};
