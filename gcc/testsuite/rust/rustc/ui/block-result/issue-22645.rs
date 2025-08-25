use std::ops::Add;

trait Scalar {}
impl Scalar for f64 {}

struct Bob;

impl<RHS: Scalar> Add <RHS> for Bob {
  type Output = Bob;
  fn add(self, rhs : RHS) -> Bob { Bob }
}

fn main() {
  let b = Bob + 3.5;
  b + 3 // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

