// PR c++/124617
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template <class T>
struct S {
  template <class U>
  static void f() {}

  static constexpr auto p = ^^f<int>;
};

constexpr void (*p)() = extract<void(*)()>(S<int>::p);
