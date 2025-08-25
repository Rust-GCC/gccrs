#![feature(raw_ref_op)]

use std::cell::Cell;

const A: () = { let x = Cell::new(2); &raw const x; };      // { dg-error ".E0492." "" { target *-*-* } }

static B: () = { let x = Cell::new(2); &raw const x; };     // { dg-error ".E0492." "" { target *-*-* } }

static mut C: () = { let x = Cell::new(2); &raw const x; }; // { dg-error ".E0492." "" { target *-*-* } }

const fn foo() {
    let x = Cell::new(0);
    let y = &raw const x;                                   // { dg-error ".E0492." "" { target *-*-* } }
}

fn main() {}

