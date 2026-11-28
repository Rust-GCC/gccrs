// ignore-tidy-linelength
#![warn(unused_attributes)] // { dg-note "" "" { target *-*-* } }

#[link_name = "foo"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
struct Foo; // { dg-note "" "" { target *-*-* } }

#[link_name = "foobar"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
extern "C" {
    fn foo() -> u32;
}
// { dg-note "" "" { target *-*-* } .-3 }

#[link_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { help "" "" { target *-*-* } .-5 }
extern "C" {
    fn bar() -> u32;
}
// { dg-note "" "" { target *-*-* } .-3 }

fn main() {}

