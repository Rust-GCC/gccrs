// run-rustfix

#![feature(staged_api)]

#![stable(since = "1.0.0", feature = "test")]

#![deny(deprecated)]
#![allow(dead_code)]

struct Foo;

impl Foo {
    #[rustc_deprecated(
        since = "1.0.0",
        reason = "replaced by `replacement`",
        suggestion = "replacement",
    )]
    #[stable(since = "1.0.0", feature = "test")]
    fn deprecated(&self) {}

    fn replacement(&self) {}
}

fn main() {
    let foo = Foo;

    foo.deprecated(); // { dg-error "" "" { target *-*-* } }
}

