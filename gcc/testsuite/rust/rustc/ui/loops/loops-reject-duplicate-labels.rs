// check-pass

// ignore-tidy-linelength

// Issue #21633: reject duplicate loop labels in function bodies.
// This is testing the exact cases that are in the issue description.

#[allow(unused_labels)]
fn foo() {
    'fl: for _ in 0..10 { break; }
    'fl: loop { break; }           // { dg-warning "" "" { target *-*-* } }

    'lf: loop { break; }
    'lf: for _ in 0..10 { break; } // { dg-warning "" "" { target *-*-* } }
    'wl: while 2 > 1 { break; }
    'wl: loop { break; }           // { dg-warning "" "" { target *-*-* } }
    'lw: loop { break; }
    'lw: while 2 > 1 { break; }    // { dg-warning "" "" { target *-*-* } }
    'fw: for _ in 0..10 { break; }
    'fw: while 2 > 1 { break; }    // { dg-warning "" "" { target *-*-* } }
    'wf: while 2 > 1 { break; }
    'wf: for _ in 0..10 { break; } // { dg-warning "" "" { target *-*-* } }
    'tl: while let Some(_) = None::<i32> { break; }
    'tl: loop { break; }           // { dg-warning "" "" { target *-*-* } }
    'lt: loop { break; }
    'lt: while let Some(_) = None::<i32> { break; }
// { dg-warning "" "" { target *-*-* } .-1 }
}

// Note however that it is okay for the same label to be reused in
// different methods of one impl, as illustrated here.

struct S;
impl S {
    fn m1(&self) { 'okay: loop { break 'okay; } }
    fn m2(&self) { 'okay: loop { break 'okay; } }
}


pub fn main() {
    let s = S;
    s.m1();
    s.m2();
    foo();
}

