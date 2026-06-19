#[derive(FromPrimitive)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
enum Foo {}

fn main() {}

