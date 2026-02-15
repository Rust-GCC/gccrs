#![crate_type="lib"]
#![feature(arbitrary_enum_discriminant)]

enum Enum {
// { dg-error ".E0732." "" { target *-*-* } .-1 }
  Unit = 1,
  Tuple() = 2,
  Struct{} = 3,
}

