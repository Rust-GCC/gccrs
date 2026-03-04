#![feature(no_core)]
#![no_core]

enum Test {
    // { dg-error "expecting" "" { target *-*-* } .+1 }
    _,
}


