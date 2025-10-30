// revisions: nll_target

// The following revisions are disabled due to missing support from two-phase beyond autorefs
//[nll_beyond] compile-flags: -Z borrowck=mir -Z two-phase-beyond-autoref

//[nll_target] compile-flags: -Z borrowck=mir

// This is an important corner case pointed out by Niko: one is
// allowed to initiate a shared borrow during a reservation, but it
// *must end* before the activation occurs.
//
// FIXME: for clarity, diagnostics for these cases might be better off
// if they specifically said "cannot activate mutable borrow of `x`"
//
// The convention for the listed revisions: "lxl" means lexical
// lifetimes (which can be easier to reason about). "nll" means
// non-lexical lifetimes. "nll_target" means the initial conservative
// two-phase borrows that only applies to autoref-introduced borrows.
// "nll_beyond" means the generalization of two-phase borrows to all
// `&mut`-borrows (doing so makes it easier to write code for specific
// corner cases).

#![allow(dead_code)]

fn read(_: &i32) { }

fn ok() {
    let mut x = 3;
    let y = &mut x;
    { let z = &x; read(z); }
// { dg-error "" "" { target *-*-* } .-1 }
    *y += 1;
}

fn not_ok() {
    let mut x = 3;
    let y = &mut x;
    let z = &x;
// { dg-error "" "" { target *-*-* } .-1 }
    *y += 1;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    read(z);
}

fn should_be_ok_with_nll() {
    let mut x = 3;
    let y = &mut x;
    let z = &x;
// { dg-error "" "" { target *-*-* } .-1 }
    read(z);
    *y += 1;
// { dg-error "" "" { target *-*-* } .-1 }
    // (okay with (generalized) nll today)
}

fn should_also_eventually_be_ok_with_nll() {
    let mut x = 3;
    let y = &mut x;
    let _z = &x;
// { dg-error "" "" { target *-*-* } .-1 }
    *y += 1;
// { dg-error "" "" { target *-*-* } .-1 }
    // (okay with (generalized) nll today)
}

fn main() { }

