// run-rustfix

#![feature(box_patterns, stmt_expr_attributes)]
#![feature(or_patterns)]

#![allow(
    dead_code,
    ellipsis_inclusive_range_patterns,
    irrefutable_let_patterns,
    unreachable_patterns,
    unused_mut,
    unused_variables
)]
#![deny(unused_parens)]

fn lint_on_top_level() {
    let (a) = 0; // { dg-error "" "" { target *-*-* } }
    for (a) in 0..1 {} // { dg-error "" "" { target *-*-* } }
    if let (a) = 0 {} // { dg-error "" "" { target *-*-* } }
    while let (a) = 0 {} // { dg-error "" "" { target *-*-* } }
    fn foo((a): u8) {} // { dg-error "" "" { target *-*-* } }
    let _ = |(a): u8| 0; // { dg-error "" "" { target *-*-* } }
}

fn _no_lint_attr() {
    let _x = #[allow(dead_code)] (1 + 2);
}

// Don't lint in these cases (#64106).
fn or_patterns_no_lint() {
    match Box::new(0) {
        box (0 | 1) => {} // Should not lint as `box 0 | 1` binds as `(box 0) | 1`.
        _ => {}
    }

    match 0 {
        x @ (0 | 1) => {} // Should not lint as `x @ 0 | 1` binds as `(x @ 0) | 1`.
        _ => {}
    }

    if let &(0 | 1) = &0 {} // Should also not lint.
    if let &mut (0 | 1) = &mut 0 {} // Same.

    fn foo((Ok(a) | Err(a)): Result<u8, u8>) {} // Doesn't parse if we remove parens for now.

    let _ = |(Ok(a) | Err(a)): Result<u8, u8>| 1; // `|Ok(a) | Err(a)| 1` parses as bit-or.
}

fn or_patterns_will_lint() {
    if let (0 | 1) = 0 {} // { dg-error "" "" { target *-*-* } }
    if let ((0 | 1),) = (0,) {} // { dg-error "" "" { target *-*-* } }
    if let [(0 | 1)] = [0] {} // { dg-error "" "" { target *-*-* } }
    if let 0 | (1 | 2) = 0 {} // { dg-error "" "" { target *-*-* } }
    struct TS(u8);
    if let TS((0 | 1)) = TS(0) {} // { dg-error "" "" { target *-*-* } }
    struct NS { f: u8 }
    if let NS { f: (0 | 1) } = (NS { f: 0 }) {} // { dg-error "" "" { target *-*-* } }
}

// Don't lint on `&(mut x)` because `&mut x` means something else (#55342).
fn deref_mut_binding_no_lint() {
    let &(mut x) = &0;
}

fn main() {
    match 1 {
        (_) => {} // { dg-error "" "" { target *-*-* } }
        (y) => {} // { dg-error "" "" { target *-*-* } }
        (ref r) => {} // { dg-error "" "" { target *-*-* } }
        (e @ 1...2) => {} // { dg-error "" "" { target *-*-* } }
        (1...2) => {} // Non ambiguous range pattern should not warn
        e @ (3...4) => {} // Non ambiguous range pattern should not warn
    }

    match &1 {
        (e @ &(1...2)) => {} // { dg-error "" "" { target *-*-* } }
        &(_) => {} // { dg-error "" "" { target *-*-* } }
        e @ &(1...2) => {} // Ambiguous range pattern should not warn
        &(1...2) => {} // Ambiguous range pattern should not warn
    }

    match &1 {
        e @ &(1...2) | e @ &(3...4) => {} // Complex ambiguous pattern should not warn
        &_ => {}
    }

    match 1 {
        (_) => {} // { dg-error "" "" { target *-*-* } }
        (y) => {} // { dg-error "" "" { target *-*-* } }
        (ref r) => {} // { dg-error "" "" { target *-*-* } }
        (e @ 1..=2) => {} // { dg-error "" "" { target *-*-* } }
        (1..=2) => {} // Non ambiguous range pattern should not warn
        e @ (3..=4) => {} // Non ambiguous range pattern should not warn
    }

    match &1 {
        (e @ &(1..=2)) => {} // { dg-error "" "" { target *-*-* } }
        &(_) => {} // { dg-error "" "" { target *-*-* } }
        e @ &(1..=2) => {} // Ambiguous range pattern should not warn
        &(1..=2) => {} // Ambiguous range pattern should not warn
    }

    match &1 {
        e @ &(1..=2) | e @ &(3..=4) => {} // Complex ambiguous pattern should not warn
        &_ => {}
    }
}

