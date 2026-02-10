// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using info = decltype(^^void);

namespace N { }

template<info R>
void
f ()
{
  int i = [:R:]; // { dg-error "expected a reflection of an expression" }
  // { dg-message "but .N. is a namespace" "" { target *-*-* } 6 }
}

void
g ()
{
  f<^^N>();
}
