#![feature(raw_ref_op)]

const A: () = { let mut x = 2; &raw mut x; };           // { dg-error ".E0764." "" { target *-*-* } }

static B: () = { let mut x = 2; &raw mut x; };          // { dg-error ".E0764." "" { target *-*-* } }

static mut C: () = { let mut x = 2; &raw mut x; };      // { dg-error ".E0764." "" { target *-*-* } }

const fn foo() {
    let mut x = 0;
    let y = &raw mut x;                                 // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

