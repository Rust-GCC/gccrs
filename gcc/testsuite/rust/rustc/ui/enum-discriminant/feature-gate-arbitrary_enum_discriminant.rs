#![crate_type="lib"]

enum Enum {
  Unit = 1,
// { dg-error ".E0658." "" { target *-*-* } .-1 }
  Tuple() = 2,
// { dg-error ".E0658." "" { target *-*-* } .-1 }
  Struct{} = 3,
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

