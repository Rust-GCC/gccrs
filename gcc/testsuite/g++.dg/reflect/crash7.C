// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using info = decltype(^^void);

struct S {
  template<typename T>
  static T t{};
} s;

template<info R>
void
f ()
{
  int j = s.template [:R:]<int>; // { dg-error "expected a reflection of an expression" }
  // { dg-message "but .S. is a type" "" { target *-*-* } 6 }
}

void
g ()
{
  f<^^S>();
}
