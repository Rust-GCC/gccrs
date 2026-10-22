// [full] run-pass
// revisions: min full
// Checks some basic test cases for defaults.
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]
#![allow(dead_code)]

struct FixedOutput<'a, const N: usize, T=u32> {
// { dg-error "" "" { target *-*-* } .-1 }
  out: &'a [T; N],
}

trait FixedOutputter {
  fn out(&self) -> FixedOutput<'_, 10>;
}

fn main() {}

