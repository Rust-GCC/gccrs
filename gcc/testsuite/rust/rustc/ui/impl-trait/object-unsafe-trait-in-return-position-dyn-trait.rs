#![allow(bare_trait_objects)]
trait NotObjectSafe {
    fn foo() -> Self;
}

struct A;
struct B;

impl NotObjectSafe for A {
    fn foo() -> Self {
        A
    }
}

impl NotObjectSafe for B {
    fn foo() -> Self {
        B
    }
}

fn car() -> dyn NotObjectSafe { // { dg-error ".E0038." "" { target *-*-* } }
    if true {
        return A;
    }
    B
}

fn cat() -> Box<dyn NotObjectSafe> { // { dg-error ".E0038." "" { target *-*-* } }
    if true {
        return Box::new(A);
    }
    Box::new(B)
}

fn main() {}

