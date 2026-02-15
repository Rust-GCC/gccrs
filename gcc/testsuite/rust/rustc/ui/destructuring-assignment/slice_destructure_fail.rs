#![feature(destructuring_assignment)]

fn main() {
  let (mut a, mut b);
  [a, .., b, ..] = [0, 1]; // { dg-error "" "" { target *-*-* } }
  [a, a, b] = [1, 2]; // { dg-error ".E0527." "" { target *-*-* } }
}

