// aux-build:derive-foo.rs
// aux-build:derive-clona.rs
// aux-build:test-macros.rs

#[macro_use]
extern crate derive_foo;
#[macro_use]
extern crate derive_clona;
extern crate test_macros;

use test_macros::empty as bang_proc_macro;
use test_macros::empty_attr as attr_proc_macro;

macro_rules! FooWithLongNam {
    () => {}
}

macro_rules! attr_proc_mac {
    () => {}
}

#[derive(FooWithLongNan)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct Foo;

// Interpreted as an unstable custom attribute
#[attr_proc_macra] // { dg-error "" "" { target *-*-* } }
struct Bar;

// Interpreted as an unstable custom attribute
#[FooWithLongNan] // { dg-error "" "" { target *-*-* } }
struct Asdf;

#[derive(Dlone)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct A;

#[derive(Dlona)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct B;

#[derive(attr_proc_macra)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct C;

fn main() {
    FooWithLongNama!();
// { dg-error "" "" { target *-*-* } .-1 }

    attr_proc_macra!();
// { dg-error "" "" { target *-*-* } .-1 }

    Dlona!();
// { dg-error "" "" { target *-*-* } .-1 }

    bang_proc_macrp!();
// { dg-error "" "" { target *-*-* } .-1 }
}

