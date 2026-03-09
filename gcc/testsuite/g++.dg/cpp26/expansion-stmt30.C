// CWG 3123
// { dg-do compile { target c++26 } }

namespace M {
  struct B {};

  template <typename T>
  auto *begin (T &t) { return &t.array[0]; }
}

namespace N {
  struct S : M::B { int array[8]; };

  template <typename T>
  auto *begin (T &s) { return &s.array[0]; }

  auto *end (const S &s) { return &s.array[8]; }
}

struct V { int begin, end; };

namespace O {
  struct B { int b; };
  struct C { int operator () (int) { return 42; } } begin, end;
}

void
foo ()
{
  for (auto i : N::S {})		// { dg-error "call of overloaded 'begin\\\(N::S\\\&\\\)' is ambiguous" }
    ;
  template for (auto i : N::S {})
    ;					// { dg-error "call of overloaded 'begin\\\(const N::S\\\&\\\)' is ambiguous" }
  template for (auto i : V {})
    ;					// { dg-error "cannot be used as a function" }
  template for (auto i : O::B {})
    ;
}
