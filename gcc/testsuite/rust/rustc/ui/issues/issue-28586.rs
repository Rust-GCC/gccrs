// Regression test for issue #28586

pub trait Foo {}
impl Foo for [u8; usize::BYTES] {}
// { dg-error ".E0599." "" { target *-*-* } .-1 }

fn main() { }

