// run-rustfix
#![allow(unused_variables, dead_code)]

trait Trait {
    fn foo() where Self: Other, { }
    fn bar(self: ()) {} // { dg-error ".E0307." "" { target *-*-* } }
}

fn bar(x: &dyn Trait) {} // { dg-error ".E0038." "" { target *-*-* } }

trait Other {}

fn main() {}

