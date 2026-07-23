// Test for issue #67776: binding named the same as enum variant
// should report a warning even when matching against a reference type

// check-pass

#![allow(unused_variables)]
#![allow(non_snake_case)]

enum Foo {
    Bar,
    Baz,
}


fn fn1(e: Foo) {
    match e {
        Bar => {},
// { dg-warning "" "" { target *-*-* } .-1 }
        Baz => {},
// { dg-warning "" "" { target *-*-* } .-1 }
    }
}

fn fn2(e: &Foo) {
    match e {
        Bar => {},
// { dg-warning "" "" { target *-*-* } .-1 }
        Baz => {},
// { dg-warning "" "" { target *-*-* } .-1 }
    }
}

fn fn3(e: &mut &&mut Foo) {
    match e {
        Bar => {},
// { dg-warning "" "" { target *-*-* } .-1 }
        Baz => {},
// { dg-warning "" "" { target *-*-* } .-1 }
    }
}

fn main() {}

