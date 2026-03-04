#![feature(no_core)]
#![no_core]

// { dg-error "expecting" "" { target *-*-* } .+1 }
enum _ {
    Variant1,
    Variant2,
}
