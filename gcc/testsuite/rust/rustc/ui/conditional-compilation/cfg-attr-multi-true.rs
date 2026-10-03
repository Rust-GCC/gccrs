// Test that cfg_attr with multiple attributes actually emits both attributes.
// This is done by emitting two attributes that cause new warnings, and then
// triggering those warnings.

// build-pass (FIXME(62277): could be check-pass?)

#![warn(unused_must_use)]

#[cfg_attr(all(), deprecated, must_use)]
struct MustUseDeprecated {}

impl MustUseDeprecated { // { dg-warning "" "" { target *-*-* } }
    fn new() -> MustUseDeprecated { // { dg-warning "" "" { target *-*-* } }
        MustUseDeprecated {} // { dg-warning "" "" { target *-*-* } }
    }
}

fn main() {
    MustUseDeprecated::new(); // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
}

