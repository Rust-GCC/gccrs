// ignore-tidy-linelength
#![deny(unused_attributes)] // { dg-note "" "" { target *-*-* } }

#[cold]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
struct Foo; // { dg-note "" "" { target *-*-* } }

fn main() {
    #[cold]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    5; // { dg-note "" "" { target *-*-* } }
}

