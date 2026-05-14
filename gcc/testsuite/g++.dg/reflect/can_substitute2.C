// [meta.reflection.substitute] Example 1
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template<class T>
auto fn1();

static_assert(!can_substitute(^^fn1, {^^int}));
constexpr auto r1 = substitute(^^fn1, {^^int}); // { dg-error "can_substitute returned false" }

template<class T>
auto fn2() {
  static_assert(false); // { dg-error "assert" }
  return T{};
}

constexpr bool r2 = can_substitute(^^fn2, {^^int}); // { dg-message "required from here" }
