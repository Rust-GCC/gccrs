#![feature(label_break_value)]

fn main() {
    // This used to ICE during liveness check because `target_id` passed to
    // `propagate_through_expr` would be the closure and not the `loop`, which wouldn't be found in
    // `self.break_ln`. (#62480)
    'a: {
        || break 'a
// { dg-error ".E0267." "" { target *-*-* } .-1 }
// { dg-error ".E0267." "" { target *-*-* } .-2 }
    }
}

