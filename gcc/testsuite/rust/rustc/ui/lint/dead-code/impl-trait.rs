#![deny(dead_code)]

trait Trait {
    type Type;
}

impl Trait for () {
    type Type = ();
}

type Used = ();
type Unused = (); // { dg-error "" "" { target *-*-* } }

fn foo() -> impl Trait<Type = Used> {}

fn main() {
    foo();
}

