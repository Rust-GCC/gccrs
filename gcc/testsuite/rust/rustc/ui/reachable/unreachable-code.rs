#![deny(unreachable_code)]
#![allow(unused_variables)]

fn main() {
  loop{}

  let a = 3; // { dg-error "" "" { target *-*-* } }
}

