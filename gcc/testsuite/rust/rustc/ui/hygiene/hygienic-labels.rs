// run-pass
#![allow(unreachable_code)]
#![allow(unused_labels)]
// Test that labels injected by macros do not break hygiene.

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

macro_rules! while_x {
    ($e: expr) => {
        // ditto
        'x: while 1 + 1 == 2 { $e }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { dg-warning "" "" { target *-*-* } .-5 }
    }
}

pub fn main() {
    'x: for _ in 0..1 {
        // this 'x should refer to the outer loop, lexically
        loop_x!(break 'x);
        panic!("break doesn't act hygienically inside for loop");
    }

    'x: loop {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

        // ditto
        loop_x!(break 'x);
        panic!("break doesn't act hygienically inside infinite loop");
    }

    'x: while 1 + 1 == 2 {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }

        while_x!(break 'x);
        panic!("break doesn't act hygienically inside infinite while loop");
    }

    'x: for _ in 0..1 {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { dg-warning "" "" { target *-*-* } .-5 }
// { dg-warning "" "" { target *-*-* } .-6 }

        // ditto
        run_once!(continue 'x);
        panic!("continue doesn't act hygienically inside for loop");
    }
}

