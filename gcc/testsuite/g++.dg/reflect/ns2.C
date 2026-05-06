// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test reflections on namespaces.  Invalid stuff.

void foo (int);
int i;
enum E { EE };
template<typename>
struct S { int i, j, k;  };
template<typename>
void bar ();

namespace N {
}

void
f1 (int p)
{
  constexpr auto r = ^^::;
  [: r :] foo (0); // { dg-error "expected a reflection of an expression" }
  // { dg-message "but .::. is a namespace" "" { target *-*-* } 0 }

  constexpr auto q = ^^int;
  [: q :]::T x; // { dg-error "expected a reflection of a class, namespace, or enumeration|expected" }
  // { dg-message "but .int. is a type" "" { target *-*-* } .-1 }
  [: ^^foo :]::X; // { dg-error "expected a reflection of a class, namespace, or enumeration" }
  // { dg-message "but .foo. is a function" "" { target *-*-* } 5 }
  [: ^^EE :]::Y;  // { dg-error "expected a reflection of a class, namespace, or enumeration" }
  // { dg-message "but .EE. is an enumerator" "" { target *-*-* } 7 }
  [: ^^S :]::Z;	  // { dg-error "expected a reflection of a class, namespace, or enumeration" }
  // { dg-message "but .S. is a class template" "" { target *-*-* } 9 }
  [: ^^bar :]::W; // { dg-error "expected a reflection of a class, namespace, or enumeration" }
  // { dg-message "but .bar. is a function template" "" { target *-*-* } 11 }
  [: ^^i :]::U;	  // { dg-error "expected a reflection of a class, namespace, or enumeration" }
  // { dg-message "but .i. is a variable" "" { target *-*-* } 6 }
  [: ^^p :]::V;	  // { dg-error "expected a reflection of a class, namespace, or enumeration" }
  // { dg-message "but .p. is a parameter" "" { target *-*-* } 17 }
  auto [a, b, c] = S<int>{1, 2, 3};
  [: ^^a :]::G;  // { dg-error "expected a reflection of a class, namespace, or enumeration" }
  // { dg-message "but .a. is a structured binding" "" { target *-*-* } 38 }
  [: ^^S<int>::i :]::H;	// { dg-error "expected a reflection of a class, namespace, or enumeration" }
  // { dg-message "but .S<int>::i. is a data member" "" { target *-*-* } 9 }

  constexpr auto x = ^^N::X;  // { dg-error ".N::X. has not been declared" }
}
