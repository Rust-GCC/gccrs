// Tests that impl can't add extra `F: Sync` bound aren't *more* restrictive
// than the trait method it's implementing.
//
// Regr test for #26111.

trait A {
  fn b<C,D>(&self, x: C) -> C;
}

struct E {
 f: isize
}

impl A for E {
    fn b<F: Sync, G>(&self, _x: F) -> F { panic!() } // { dg-error ".E0276." "" { target *-*-* } }
}

fn main() {}

