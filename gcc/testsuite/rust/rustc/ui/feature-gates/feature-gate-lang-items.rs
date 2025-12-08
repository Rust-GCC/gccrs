#[lang = "foo"] // { dg-error ".E0522." "" { target *-*-* } }
// { dg-error ".E0522." "" { target *-*-* } .-1 }
trait Foo {}

fn main() {}

