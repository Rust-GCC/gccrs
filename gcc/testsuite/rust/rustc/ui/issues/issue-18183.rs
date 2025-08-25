pub struct Foo<Bar=Bar>(Bar); // { dg-error ".E0128." "" { target *-*-* } }
pub struct Baz(Foo);
fn main() {}

