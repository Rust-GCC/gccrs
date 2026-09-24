#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

fn main() {}

trait X {
    default const A: u8; // { dg-error "" "" { target *-*-* } }
    default const B: u8 = 0;  // { dg-error "" "" { target *-*-* } }
    default type D; // { dg-error "" "" { target *-*-* } }
    default type C: Ord; // { dg-error "" "" { target *-*-* } }
    default fn f1(); // { dg-error "" "" { target *-*-* } }
    default fn f2() {} // { dg-error "" "" { target *-*-* } }
}

