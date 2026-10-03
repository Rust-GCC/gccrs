fn main() {}

const Z: () = panic!("cheese");
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const Y: () = unreachable!();
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const X: () = unimplemented!();
// { dg-error ".E0658." "" { target *-*-* } .-1 }

