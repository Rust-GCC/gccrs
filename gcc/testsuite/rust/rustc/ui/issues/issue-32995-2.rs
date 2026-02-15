fn main() {
    { fn f<X: ::std::marker()::Send>() {} }
// { dg-error ".E0214." "" { target *-*-* } .-1 }

    { fn f() -> impl ::std::marker()::Send { } }
// { dg-error ".E0214." "" { target *-*-* } .-1 }
}

#[derive(Clone)]
struct X;

impl ::std::marker()::Copy for X {}
// { dg-error ".E0214." "" { target *-*-* } .-1 }

