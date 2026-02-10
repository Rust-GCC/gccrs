// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

int g;

void
fn0 (int, typename [: ^^:: :] i) // { dg-error "expected a reflection of a type|declared" }
// { dg-message "but .::. is a namespace" "" { target *-*-* } 0 }
{
}

void
fn1 (int, typename [: ^^g :] i) // { dg-error "expected a reflection of a type|declared" }
// { dg-message "but .g. is a variable" "" { target *-*-* } 4 }
{
}

void
fn2 (int, typename [: ^^fn1 :] i) // { dg-error "expected a reflection of a type|declared" }
// { dg-message "but .fn1. is a function" "" { target *-*-* } 13 }
{
}

void
fn3 (int p, typename [: ^^p :] i) // { dg-error "expected a reflection of a type|declared" }
// { dg-message "but .p. is a parameter" "" { target *-*-* } 25 }
{
}

enum Harold { Budd };

void
fn4 (int, typename [: ^^Budd :] i) // { dg-error "expected a reflection of a type|declared" }
// { dg-message "but .Budd. is an enumerator" "" { target *-*-* } 30 }
{
}

template<int>
struct S {};

void
fn5 (int, typename [: ^^S :] i) // { dg-error "expected a reflection of a type|declared" }
// { dg-message "but .S. is a class template" "" { target *-*-* } 39 }
{
}

template<int>
void bar ();

void
fn6 (int, typename [: ^^bar :] i) // { dg-error "expected a reflection of a type|declared" }
// { dg-message "but .bar. is a function template" "" { target *-*-* } 48 }
{
}
