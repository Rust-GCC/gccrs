// PR c++/125745
// { dg-do compile { target c++11 } }
// { dg-additional-options -fsanitize=undefined }

struct A { virtual void foo (); };
struct B : A { constexpr int foo () const { return 42; } };
constexpr B x;
struct C { int i; C () : i (x.foo ()) {} };
