fn foo(x: &'a str) { } // { dg-error ".E0261." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

struct Foo {
    x: &'a str, // { dg-error ".E0261." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

