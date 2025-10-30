fn main() {}

trait Foo {
    default!(); // { dg-error "" "" { target *-*-* } }
    default do
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

struct S;
impl S {
    default!(); // { dg-error "" "" { target *-*-* } }
    default do
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

