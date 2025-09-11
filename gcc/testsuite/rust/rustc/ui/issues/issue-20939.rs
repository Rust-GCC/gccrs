trait Foo {}

impl<'a> Foo for dyn Foo + 'a {}
// { dg-error ".E0371." "" { target *-*-* } .-1 }

fn main() {}

