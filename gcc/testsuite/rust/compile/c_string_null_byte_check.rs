// { dg-additional-options "-frust-c-style-string-literals" }
#![feature(no_core)]
#![no_core]

pub fn main() {
    let _fail = c"gc\0crs";
    // { dg-error "null characters in C string literals are not supported" "" { target *-*-* } .-1 }
}