// gate-test-allow_internal_unsafe

#![allow(unused_macros)]

macro_rules! bar {
    () => {
        // more layers don't help:
        #[allow_internal_unsafe] // { dg-error ".E0658." "" { target *-*-* } }
        macro_rules! baz {
            () => {}
        }
    }
}

bar!();

fn main() {}

