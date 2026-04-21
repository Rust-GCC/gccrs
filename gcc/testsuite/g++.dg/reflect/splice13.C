// PR c++/124944
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

struct A {};
constexpr auto x = ^^A;

struct B : [: x :] {
  B () : typename [: x :] () {}
};

struct C : [: x :] {
  C () : [: x :] () {}
};

namespace D {
  struct E {};
}
constexpr auto y = ^^D;

struct F : [: y :]::E {
  F () : [: y :]::E () {}
};
