// run-pass
#![allow(unreachable_code)]
#![allow(unused_labels)]

// Test that labels injected by macros do not break hygiene.  This
// checks cases where the macros invocations are under the rhs of a
// let statement.

// Issue #24278: The label/lifetime shadowing checker from #24162
// conservatively ignores hygiene, and thus issues warnings that are
// both true- and false-positives for this test.

macro_rules! loop_x {
    ($e: expr) => {
        // $e shouldn't be able to interact with this 'x
        'x: loop { $e }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
    }
}

macro_rules! while_true {
    ($e: expr) => {
        // $e shouldn't be able to interact with this 'x
        'x: while 1 + 1 == 2 { $e }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { dg-warning "" "" { target *-*-* } .-5 }
    }
}

macro_rules! run_once {
    ($e: expr) => {
        // ditto
        'x: for _ in 0..1 { $e }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { dg-warning "" "" { target *-*-* } .-5 }
// { dg-warning "" "" { target *-*-* } .-6 }
// { dg-warning "" "" { target *-*-* } .-7 }
    }
}

pub fn main() {
    let mut i = 0;

    let j: isize = {
        'x: loop {
            // this 'x should refer to the outer loop, lexically
            loop_x!(break 'x);
            i += 1;
        }
        i + 1
    };
    assert_eq!(j, 1);

    let k: isize = {
        'x: for _ in 0..1 {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
            // ditto
            loop_x!(break 'x);
            i += 1;
        }
        i + 1
    };
    assert_eq!(k, 1);

    let l: isize = {
        'x: for _ in 0..1 {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
            // ditto
            while_true!(break 'x);
            i += 1;
        }
        i + 1
    };
    assert_eq!(l, 1);

    let n: isize = {
        'x: for _ in 0..1 {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { dg-warning "" "" { target *-*-* } .-5 }
// { dg-warning "" "" { target *-*-* } .-6 }
            // ditto
            run_once!(continue 'x);
            i += 1;
        }
        i + 1
    };
    assert_eq!(n, 1);
}

