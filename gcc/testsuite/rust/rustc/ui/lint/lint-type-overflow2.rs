// compile-flags: -O

#![deny(overflowing_literals)]
#![deny(const_err)]

fn main() {
    let x2: i8 = --128; // { dg-error "" "" { target *-*-* } }

    let x = -3.40282357e+38_f32; // { dg-error "" "" { target *-*-* } }
    let x =  3.40282357e+38_f32; // { dg-error "" "" { target *-*-* } }
    let x = -1.7976931348623159e+308_f64; // { dg-error "" "" { target *-*-* } }
    let x =  1.7976931348623159e+308_f64; // { dg-error "" "" { target *-*-* } }
}

