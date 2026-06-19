#![feature(destructuring_assignment)]

const C: i32 = 1;

fn main() {
    let (mut a, mut b);
    (a, .., b, ..) = (0, 1); // { dg-error "" "" { target *-*-* } }
    (a, a, b) = (1, 2); // { dg-error ".E0308." "" { target *-*-* } }
    (C, ..) = (0,1); // { dg-error ".E0070." "" { target *-*-* } }
}

