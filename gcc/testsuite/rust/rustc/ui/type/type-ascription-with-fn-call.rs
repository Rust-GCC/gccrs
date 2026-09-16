// run-rustfix
#![feature(type_ascription)]

fn main() {
    f()  :
    f(); // { dg-error ".E0573." "" { target *-*-* } }
}

fn f() {}

