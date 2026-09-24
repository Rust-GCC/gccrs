// Test `X...` and `X..=` range patterns not being allowed syntactically.
// FIXME(Centril): perhaps these should be semantic restrictions.

#![feature(half_open_range_patterns)]

fn main() {}

#[cfg(FALSE)]
fn foo() {
    if let 0... = 1 {} // { dg-error ".E0586." "" { target *-*-* } }
    if let 0..= = 1 {} // { dg-error ".E0586." "" { target *-*-* } }
    const X: u8 = 0;
    if let X... = 1 {} // { dg-error ".E0586." "" { target *-*-* } }
    if let X..= = 1 {} // { dg-error ".E0586." "" { target *-*-* } }
}

fn bar() {
    macro_rules! mac {
        ($e:expr) => {
            let $e...; // { dg-error ".E0586." "" { target *-*-* } }
            let $e..=; // { dg-error ".E0586." "" { target *-*-* } }
        }
    }

    mac!(0);
}

