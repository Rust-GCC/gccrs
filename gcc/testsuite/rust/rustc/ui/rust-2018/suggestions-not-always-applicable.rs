// aux-build:suggestions-not-always-applicable.rs
// edition:2015
// run-rustfix
// rustfix-only-machine-applicable
// check-pass

#![feature(rust_2018_preview)]
#![warn(rust_2018_compatibility)]

extern crate suggestions_not_always_applicable as foo;

pub struct Foo;

mod test {
    use crate::foo::foo;

    #[foo] // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
    fn main() {
    }
}

fn main() {
    test::foo();
}

