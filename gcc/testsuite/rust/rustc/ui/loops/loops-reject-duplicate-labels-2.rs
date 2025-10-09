// check-pass

// ignore-tidy-linelength

// Issue #21633: reject duplicate loop labels in function bodies.
//
// This is testing the generalization (to the whole function body)
// discussed here:
// https://internals.rust-lang.org/t/psa-rejecting-duplicate-loop-labels/1833

#[allow(unused_labels)]
pub fn foo() {
    { 'fl: for _ in 0..10 { break; } }
    { 'fl: loop { break; } }             // { dg-warning "" "" { target *-*-* } }
    { 'lf: loop { break; } }
    { 'lf: for _ in 0..10 { break; } }   // { dg-warning "" "" { target *-*-* } }
    { 'wl: while 2 > 1 { break; } }
    { 'wl: loop { break; } }             // { dg-warning "" "" { target *-*-* } }
    { 'lw: loop { break; } }
    { 'lw: while 2 > 1 { break; } }      // { dg-warning "" "" { target *-*-* } }
    { 'fw: for _ in 0..10 { break; } }
    { 'fw: while 2 > 1 { break; } }      // { dg-warning "" "" { target *-*-* } }
    { 'wf: while 2 > 1 { break; } }
    { 'wf: for _ in 0..10 { break; } }   // { dg-warning "" "" { target *-*-* } }
    { 'tl: while let Some(_) = None::<i32> { break; } }
    { 'tl: loop { break; } }             // { dg-warning "" "" { target *-*-* } }
    { 'lt: loop { break; } }
    { 'lt: while let Some(_) = None::<i32> { break; } }
// { dg-warning "" "" { target *-*-* } .-1 }
}


pub fn main() {
    foo();
}

