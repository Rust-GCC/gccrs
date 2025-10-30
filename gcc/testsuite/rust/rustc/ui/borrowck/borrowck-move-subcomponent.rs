// Tests that the borrow checker checks all components of a path when moving
// out.

#![feature(box_syntax)]

struct S {
  x : Box<isize>
}

fn f<T>(_: T) {}

fn main() {
  let a : S = S { x : box 1 };
  let pb = &a;
  let S { x: ax } = a;  // { dg-error ".E0505." "" { target *-*-* } }
  f(pb);
}

