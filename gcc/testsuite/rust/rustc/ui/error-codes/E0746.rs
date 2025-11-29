// run-rustfix
#![allow(dead_code)]
struct Struct;
trait Trait {}
impl Trait for Struct {}
impl Trait for u32 {}

fn foo() -> dyn Trait { Struct }
// { dg-error ".E0746." "" { target *-*-* } .-1 }

fn bar() -> dyn Trait { // { dg-error ".E0746." "" { target *-*-* } }
    if true {
        return 0;
    }
    42
}

fn main() {}

