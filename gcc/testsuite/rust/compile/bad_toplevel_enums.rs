pub enum E
{
  pub A { a: i32 },
  B (u8),
  pub C,
  B // { dg-error "redefined" }
}

enum E2 { }
struct E2 { } // { dg-error "redefined" }

enum E1
{
  A,
  pub B = 42,
  C = 3,
  A { a: u8 }, // { dg-error "redefined" }
  pub D
}
