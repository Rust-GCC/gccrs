// { dg-options "-frust-incomplete-and-experimental-compiler-do-not-use" }
#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
trait Sized {}

extern "C" {
    fn some_func<T>(x: T);
    // { dg-error "foreign items may not have type parameters" "" { target *-*-* } .-1 }
    // { dg-message "replace the type parameters with concrete types" "" { target *-*-* } .-2 }
}
fn main() {}