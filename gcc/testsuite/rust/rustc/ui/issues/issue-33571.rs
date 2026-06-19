#[derive(Clone,
         Sync, // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
         Copy)]
enum Foo {}

fn main() {}

