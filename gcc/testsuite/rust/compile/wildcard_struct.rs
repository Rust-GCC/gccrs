#![feature(no_core)]
#![no_core]

// { dg-error "expecting" "" { target *-*-* } .+1 }
struct _ {
    a : i32,
}
