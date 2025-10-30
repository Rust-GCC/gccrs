// Test that we have enough false edges to avoid exposing the exact matching
// algorithm in borrow checking.

#![feature(nll)]

fn all_previous_tests_may_be_done(y: &mut (bool, bool)) {
    let r = &mut y.1;
    // We don't actually test y.1 to select the second arm, but we don't want
    // borrowck results to be based on the order we match patterns.
    match y {
        (false, true) => 1, // { dg-error ".E0503." "" { target *-*-* } }
        (true, _) => {
            r;
            2
        }
        (false, _) => 3,
    };
}

fn main() {}

