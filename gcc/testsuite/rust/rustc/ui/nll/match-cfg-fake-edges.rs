// Test that we have enough false edges to avoid exposing the exact matching
// algorithm in borrow checking.

fn guard_always_precedes_arm(y: i32) {
    let mut x;
    // x should always be initialized, as the only way to reach the arm is
    // through the guard.
    match y {
        0 | 2 if { x = 2; true } => x,
        _ => 2,
    };
}

fn guard_may_be_skipped(y: i32) {
    let x;
    // Even though x *is* always initialized, we don't want to have borrowck
    // results be based on whether patterns are exhaustive.
    match y {
        _ if { x = 2; true } => 1,
        _ if {
            x; // { dg-error ".E0381." "" { target *-*-* } }
            false
        } => 2,
        _ => 3,
    };
}

fn guard_may_be_taken(y: bool) {
    let x = String::new();
    // Even though x *is* never moved before the use, we don't want to have
    // borrowck results be based on whether patterns are disjoint.
    match y {
        false if { drop(x); true } => 1,
        true => {
            x; // { dg-error ".E0382." "" { target *-*-* } }
            2
        }
        false => 3,
    };
}

fn main() {}

