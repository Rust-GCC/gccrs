#![feature(no_core)]
#![no_core]

pub enum E
{
  pub A { a: i32 },
  B (u8),
  pub C,
  B // { dg-error "defined multiple times" }
}

enum E2 { }
struct E2 { } // { dg-error "defined multiple times" }

enum E1
{
  A,
  pub B = 42, // { dg-error "custom discriminant values are not allowed in enums with tuple or struct variants." }
  C = 3, // { dg-error "custom discriminant values are not allowed in enums with tuple or struct variants." }
  A { a: u8 }, // { dg-error "defined multiple times" }
  pub D
}
